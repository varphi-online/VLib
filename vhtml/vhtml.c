#include "vhtml_h.h"
#include <stdalign.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ------------- UTF8 PARSING ----------------

#define UTF8_C(code)\
  (((code) <= 0x7F) ? (code) :\
  ((code) <= 0x7FF) ? ((0xC0 | ((code) >> 6)) << 8) | (0x80 | ((code) & 0x3F)):\
  ((code) >= 0xD800 && (code) <= 0xDFFF) ? 0 :\
  ((code) <= 0xFFFF) ? ((0xE0 | ((code) >> 12)) << 16) | \
	  ((0x80 | (((code) >> 6) & 0x3F)) << 8) | (0x80 | ((code) & 0x3F)):\
  ((code) <= 0x10FFFF) ? ((0xF0 | ((code) >> 18)) << 24) | \
	  ((0x80 | (((code) >> 12) & 0x3F)) << 16) | ((0x80 | (((code) >> 6) & 0x3F)) << 8)\
	  | (0x80 | ((code) & 0x3F)): 0)

static inline char width_utf8(char p){
    return 1 + ((unsigned char)p >= 0xC0) + ((unsigned char)p >= 0xE0) + ((unsigned char)p >= 0xF0);
}

/*
 * Returns the address of the next codepoint make sure p is not null
 * */
static inline char *next_char_utf8(char *p, char *width) {
  unsigned c = (unsigned char)*p;
  if(c >= 0x80 && c <= 0xBF) return NULL; // got the middle of a mb char
  //       ascii    dualbyte       tribyte      quadbyte
  char _width = 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
  if(width) *width = _width;
  return p + _width;
}

static const uint32_t tribyte_ws[] = {
	(uint32_t)UTF8_C(0x1680), // Ogham Space Mark
	// U+2000-U+200A various whitespace check w/ range
	(uint32_t)UTF8_C(0x2028), // Line Seperator
	(uint32_t)UTF8_C(0x2029), // Paragragraph Seperator
	(uint32_t)UTF8_C(0x202F), // Narrow NBSP
	(uint32_t)UTF8_C(0x205F), // Medium Math Space
	(uint32_t)UTF8_C(0x3000), // Ideographic Space
};

static inline int is_ws_utf8(const char *p, int codepointwidth) {
    const unsigned char *s = (const unsigned char *)p;
    if (__builtin_expect(!s || !*s, 0)) return 0;

    // Fast path: ASCII branchless & no jump table.
    if (__builtin_expect(codepointwidth == 1, 1)) {
        unsigned c = s[0];
        // ' ' (0x20) OR one of \t \n \v \f \r (0x09..0x0D)
        return c == ' ' || (unsigned)(c - '\t') <= (unsigned)('\r' - '\t');
    }

    switch (codepointwidth) {
    case 2: {
        uint32_t cpt = ((uint32_t)s[0] << 8) | s[1];
        return cpt == (uint32_t)UTF8_C(0x85)
            || cpt == (uint32_t)UTF8_C(0xA0);
    }
    case 3: {
        uint32_t cpt = ((uint32_t)s[0] << 16)
                     | ((uint32_t)s[1] <<  8)
                     |  (uint32_t)s[2];
        // single unsigned compare for the 0x2000..0x200A range
        if ((cpt - UTF8_C(0x2000))
            <= (UTF8_C(0x200A) - UTF8_C(0x2000))) return 1;
        for (int i = 0; i < 6; i++) {
            if (cpt == tribyte_ws[i]) return 1;
        }
        return 0;
    }
    case 4: return 0;
    }
    return -1;
}

// skip ws, starting from the next char, *buf ends up on first non-whitespace character (x)___(x)xxx
// if the next character is not whitespace, nothing is done
static inline int skip_whitespace(char **buf){
	int bytes_read = 0;
	char curr_wid;
	for(;;){
		unsigned char c = (unsigned)**buf;
        if(c == ' ' || (unsigned)(c - '\t') <= (unsigned)('\r' - '\t')){
			*buf += 1;
			bytes_read++;
			continue;
		}
		curr_wid = width_utf8(**buf);
		if(!is_ws_utf8(*buf, curr_wid)) break;
		*buf += curr_wid;
		bytes_read += curr_wid;
	}
	return bytes_read;
}

// skip non-whitespace, starting from the next char, *buf ends up first whitespace character (_)xxx(_)___
// if the next character is whitespace, nothing is done
static inline int skip_characters(char **buf){
	int bytes_read = 0;
	char *next_char, curr_wid;
	if(next_char = next_char_utf8(*buf, &curr_wid), next_char && is_ws_utf8(next_char, curr_wid))
		return 0;
	while(next_char = next_char_utf8(*buf, &curr_wid), next_char && !is_ws_utf8(next_char, curr_wid)){
		*buf = next_char;
		bytes_read += curr_wid;
	}
	*buf = next_char_utf8(*buf, 0);
	if(!*buf) return -1; /*EOB reached*/
	return bytes_read;
}

// --------------- END UTF8 PARSING ----------

// --------------- ALLOCATION STUFF ----------



// Allocates an arena of size bytes
VHTMLArena *arena_init(size_t size){
	if(!size) return NULL;
	VHTMLArena *arena = VHTML_MALLOC(sizeof(VHTMLArena));
	if(!arena) return NULL;
	struct VHTMLArenaChunk *c = VHTML_CALLOC(1, sizeof(struct VHTMLArenaChunk));
	c->size = size;
	c->data = VHTML_MALLOC(size);
	if(!c->data){
		VHTML_FREE(c);
		VHTML_FREE(arena);
		return NULL;
	}
	arena->head = c;
	arena->tail = c;
	return arena;
}

static struct VHTMLArenaChunk *chunk_init(size_t size){
	if(!size) return NULL;
	struct VHTMLArenaChunk *chunk = VHTML_CALLOC(1, sizeof(struct VHTMLArenaChunk));
	if(!chunk) return NULL;
	chunk->size = size;
	chunk->data = VHTML_MALLOC(size);
	if(!chunk->data){
		VHTML_FREE(chunk);
		return NULL;
	}
	return chunk;
}

void arena_free(VHTMLArena *arena){
		struct VHTMLArenaChunk *a = arena->head;
		while(a){
			struct VHTMLArenaChunk *next = a->next;
			VHTML_FREE(a->data);
			VHTML_FREE(a);
			a = next;
		}
		VHTML_FREE(arena);
}

static size_t align_up(size_t n, size_t align) {
	size_t rem = n % align;
	return rem ? (n + (align - rem)) : n;
}

// Returns
static void* arena_alloc(VHTMLArena *arena, size_t size, size_t alignment){
	if(!arena || !size) return NULL;
	struct VHTMLArenaChunk *chunk = arena->tail;
	size_t used = align_up(chunk->used, alignment ? alignment : alignof(max_align_t)); // align to next boundary
	
    if (used + size > chunk->size) {
        // 1. Try to reuse an existing downstream chunk
		struct VHTMLArenaChunk *probe = chunk->next;
		while (probe && size > probe->size) probe = probe->next;
		if (probe) {
		    arena->tail = probe;
		    chunk = probe;
		    chunk->used = 0;
		} else {
		    // splice on the actual end of chain
		    struct VHTMLArenaChunk *end = chunk;
		    while (end->next) end = end->next;
		    size_t nsz = (size > chunk->size) ? size * 2 : chunk->size;
		    struct VHTMLArenaChunk *new_chunk = chunk_init(nsz);
		    if (!new_chunk) return NULL;
		    end->next = new_chunk;
		    arena->tail = new_chunk;
		    chunk = new_chunk;
		}
        used = align_up(chunk->used, alignment ? alignment : alignof(max_align_t));
    }

	// actually bump alloc
	void *out = chunk->data + used;
    chunk->used = used + size;
    return out;
}

void arena_reset(VHTMLArena *arena) {
    if (!arena) return;
    struct VHTMLArenaChunk *c = arena->head;
    while (c) {
        c->used = 0;
        c = c->next;
    }
    arena->tail = arena->head;
}

// --------------- END ALLOCATION STUFF ----------

const char *const html_tag_str[_VHTML_TAG_COUNT] = {
	[VHTMLTAG_DOCTYPE] = "!DOCTYPE",
	[VHTMLTAG_HTML] = "html", [VHTMLTAG_BASE] = "base", [VHTMLTAG_HEAD] = "head", [VHTMLTAG_LINK] = "link", [VHTMLTAG_META] = "meta",
	[VHTMLTAG_STYLE] = "style", [VHTMLTAG_TITLE] = "title", [VHTMLTAG_BODY] = "body", [VHTMLTAG_ADDRESS] = "address", [VHTMLTAG_ARTICLE] = "article",
	[VHTMLTAG_ASIDE] = "aside", [VHTMLTAG_FOOTER] = "footer", [VHTMLTAG_HEADER] = "header", [VHTMLTAG_H1] = "h1", [VHTMLTAG_H2] = "h2", [VHTMLTAG_H3] = "h3",
	[VHTMLTAG_H4] = "h4", [VHTMLTAG_H5] = "h5", [VHTMLTAG_H6] = "h6", [VHTMLTAG_HGROUP] = "hgroup", [VHTMLTAG_MAIN] = "main", [VHTMLTAG_NAV] = "nav",
	[VHTMLTAG_SECTION] = "section", [VHTMLTAG_SEARCH] = "search", [VHTMLTAG_BLOCKQUOTE] = "blockquote", [VHTMLTAG_DD] = "dd", [VHTMLTAG_DIV] = "div",
	[VHTMLTAG_DL] = "dl", [VHTMLTAG_DT] = "dt", [VHTMLTAG_FIGCAPTION] = "figcaption", [VHTMLTAG_FIGURE] = "figure", [VHTMLTAG_HR] = "hr", [VHTMLTAG_LI] = "li",
	[VHTMLTAG_MENU] = "menu", [VHTMLTAG_OL] = "ol", [VHTMLTAG_P] = "p", [VHTMLTAG_PRE] = "pre", [VHTMLTAG_UL] = "ul", [VHTMLTAG_A] = "a", [VHTMLTAG_ABBR] = "abbr",
	[VHTMLTAG_B] = "b", [VHTMLTAG_BDI] = "bdi", [VHTMLTAG_BDO] = "bdo", [VHTMLTAG_BR] = "br", [VHTMLTAG_CITE] = "cite", [VHTMLTAG_CODE] = "code", [VHTMLTAG_DATA] = "data",
	[VHTMLTAG_DFN] = "dfn", [VHTMLTAG_EM] = "em", [VHTMLTAG_I] = "i", [VHTMLTAG_KBD] = "kbd", [VHTMLTAG_MARK] = "mark", [VHTMLTAG_G] = "g", [VHTMLTAG_RP] = "rp",
	[VHTMLTAG_RT] = "rt", [VHTMLTAG_RUBY] = "ruby", [VHTMLTAG_S] = "s", [VHTMLTAG_SAMP] = "samp", [VHTMLTAG_SMALL] = "small", [VHTMLTAG_SPAN] = "span",
	[VHTMLTAG_STRONG] = "strong", [VHTMLTAG_SUB] = "sub", [VHTMLTAG_SUP] = "sup", [VHTMLTAG_TIME] = "time", [VHTMLTAG_U] = "u", [VHTMLTAG_VAR] = "var",
	[VHTMLTAG_WBR] = "wbr", [VHTMLTAG_AREA] = "area", [VHTMLTAG_AUDIO] = "audio", [VHTMLTAG_IMG] = "img", [VHTMLTAG_MAP] = "map", [VHTMLTAG_TRACK] = "track",
	[VHTMLTAG_VIDEO] = "video", [VHTMLTAG_EMBED] = "embed", [VHTMLTAG_FENCEDFRAME] = "fencedframe", [VHTMLTAG_IFRAME] = "iframe", [VHTMLTAG_OBJECT] = "object",
	[VHTMLTAG_PICTURE] = "picture", [VHTMLTAG_SOURCE] = "source", [VHTMLTAG_SVG] = "svg", [VHTMLTAG_MATH] = "math", [VHTMLTAG_CANVAS] = "canvas",
	[VHTMLTAG_NOSCRIPT] = "noscript", [VHTMLTAG_SCRIPT] = "script", [VHTMLTAG_DEL] = "del", [VHTMLTAG_INS] = "ins", [VHTMLTAG_CAPTION] = "caption",
	[VHTMLTAG_COL] = "col", [VHTMLTAG_COLGROUP] = "colgroup", [VHTMLTAG_TABLE] = "table", [VHTMLTAG_TBODY] = "tbody", [VHTMLTAG_TD] = "td",
	[VHTMLTAG_TFOOT] = "tfoot", [VHTMLTAG_TH] = "th", [VHTMLTAG_THEAD] = "thead", [VHTMLTAG_TR] = "tr", [VHTMLTAG_BUTTON] = "button", [VHTMLTAG_DATALIST] = "datalist",
	[VHTMLTAG_FIELDSET] = "fieldset", [VHTMLTAG_FORM] = "form", [VHTMLTAG_INPUT] = "input", [VHTMLTAG_LABEL] = "label", [VHTMLTAG_LEGEND] = "legend",
	[VHTMLTAG_METER] = "meter", [VHTMLTAG_OPTGROUP] = "optgroup", [VHTMLTAG_OPTION] = "option", [VHTMLTAG_OUTPUT] = "output", [VHTMLTAG_PROGRESS] = "progress",
	[VHTMLTAG_SELECT] = "select", [VHTMLTAG_SELECTEDCONTENT] = "selectedcontent", [VHTMLTAG_TEXTAREA] = "textarea", [VHTMLTAG_DETAILS] = "details",
	[VHTMLTAG_DIALOG] = "dialog", [VHTMLTAG_GEOLOCATION] = "geolocation", [VHTMLTAG_SUMMARY] = "summary", [VHTMLTAG_SLOT] = "slot", [VHTMLTAG_TEMPLATE] = "template",
	[VHTMLTAG_ACRONYM] = "acronym", [VHTMLTAG_BIG] = "big", [VHTMLTAG_CENTER] = "center", [VHTMLTAG_CONTENT] = "content", [VHTMLTAG_DIR] = "dir", [VHTMLTAG_FONT] = "font",
	[VHTMLTAG_FRAME] = "frame", [VHTMLTAG_FRAMESET] = "frameset", [VHTMLTAG_IMAGE] = "image", [VHTMLTAG_MARQUEE] = "marquee", [VHTMLTAG_MENUITEM] = "menuitem",
	[VHTMLTAG_NOBR] = "nobr", [VHTMLTAG_NOEMBED] = "noembed", [VHTMLTAG_NOFRAMES] = "noframes", [VHTMLTAG_PARAM] = "param", [VHTMLTAG_PLAINTEXT] = "plaintext",
	[VHTMLTAG_RB] = "rb", [VHTMLTAG_RTC] = "rtc", [VHTMLTAG_SHADOW] = "shadow", [VHTMLTAG_STRIKE] = "strike", [VHTMLTAG_TT] = "tt", [VHTMLTAG_XMP] = "xmp",
	[_VHTMLTAG_TEXT] = "_innerText"
};

const char *const html_attribute_str[_VHTML_ATTRIBUTE_COUNT] = {
	[VHTMLATTR_ACCEPT] = "accept", [VHTMLATTR_ACCEPTCHARSET] = "accept-charset", [VHTMLATTR_ACCESSKEY] = "accesskey", [VHTMLATTR_ACTION] = "action", 
	[VHTMLATTR_ALIGN] = "align", [VHTMLATTR_ALT] = "alt", [VHTMLATTR_ASYNC] = "async", [VHTMLATTR_AUTOCOMPLETE] = "autocomplete", 
	[VHTMLATTR_AUTOFOCUS] = "autofocus", [VHTMLATTR_AUTOPLAY] = "autoplay", [VHTMLATTR_BGCOLOR] = "bgcolor", [VHTMLATTR_BORDER] = "border", 
	[VHTMLATTR_CHARSET] = "charset", [VHTMLATTR_CHECKED] = "checked", [VHTMLATTR_CITE] = "cite", [VHTMLATTR_CLASS] = "class", [VHTMLATTR_COLOR] = "color", 
	[VHTMLATTR_COLS] = "cols", [VHTMLATTR_COLSPAN] = "colspan", [VHTMLATTR_CONTENT] = "content", [VHTMLATTR_CONTENTEDITABLE] = "contenteditable", 
	[VHTMLATTR_CONTROLS] = "controls", [VHTMLATTR_COORDS] = "coords", [VHTMLATTR_DATA] = "data", [VHTMLATTR_DATETIME] = "datetime", [VHTMLATTR_DEFAULT] = "default", 
	[VHTMLATTR_DEFER] = "defer", [VHTMLATTR_DIR] = "dir", [VHTMLATTR_DIRNAME] = "dirname", [VHTMLATTR_DISABLED] = "disabled", [VHTMLATTR_DOWNLOAD] = "download", 
	[VHTMLATTR_DRAGGABLE] = "draggable", [VHTMLATTR_ENCTYPE] = "enctype", [VHTMLATTR_ENTERKEYHINT] = "enterkeyhint", [VHTMLATTR_FOR] = "for", [VHTMLATTR_FORM] = "form", 
	[VHTMLATTR_FORMACTION] = "formaction", [VHTMLATTR_HEADERS] = "headers", [VHTMLATTR_HEIGHT] = "height", [VHTMLATTR_HIDDEN] = "hidden", [VHTMLATTR_HIGH] = "high", 
	[VHTMLATTR_HREF] = "href", [VHTMLATTR_HREFLANG] = "hreflang", [VHTMLATTR_HTTPEQUIV] = "http-equiv", [VHTMLATTR_ID] = "id", [VHTMLATTR_INERT] = "inert", 
	[VHTMLATTR_INPUTMODE] = "inputmode", [VHTMLATTR_ISMAP] = "ismap", [VHTMLATTR_KIND] = "kind", [VHTMLATTR_LABEL] = "label", [VHTMLATTR_LANG] = "lang", 
	[VHTMLATTR_LIST] = "list", [VHTMLATTR_LOOP] = "loop", [VHTMLATTR_LOW] = "low", [VHTMLATTR_MAX] = "max", [VHTMLATTR_MAXLENGTH] = "maxlength", [VHTMLATTR_MEDIA] = "media", 
	[VHTMLATTR_METHOD] = "method", [VHTMLATTR_MIN] = "min", [VHTMLATTR_MULTIPLE] = "multiple", [VHTMLATTR_MUTED] = "muted", [VHTMLATTR_NAME] = "name", 
	[VHTMLATTR_NOVALIDATE] = "novalidate", [VHTMLATTR_ONABORT] = "onabort", [VHTMLATTR_ONAFTERPRINT] = "onafterprint", [VHTMLATTR_ONBEFOREPRINT] = "onbeforeprint", 
	[VHTMLATTR_ONBEFOREUNLOAD] = "onbeforeunload", [VHTMLATTR_ONBLUR] = "onblur", [VHTMLATTR_ONCANPLAY] = "oncanplay", [VHTMLATTR_ONCANPLAYTHROUGH] = "oncanplaythrough", 
	[VHTMLATTR_ONCHANGE] = "onchange", [VHTMLATTR_ONCLICK] = "onclick", [VHTMLATTR_ONCONTEXTMENU] = "oncontextmenu", [VHTMLATTR_ONCOPY] = "oncopy", 
	[VHTMLATTR_ONCUECHANGE] = "oncuechange", [VHTMLATTR_ONCUT] = "oncut", [VHTMLATTR_ONDBLCLICK] = "ondblclick", [VHTMLATTR_ONDRAG] = "ondrag", 
	[VHTMLATTR_ONDRAGEND] = "ondragend", [VHTMLATTR_ONDRAGENTER] = "ondragenter", [VHTMLATTR_ONDRAGLEAVE] = "ondragleave", [VHTMLATTR_ONDRAGOVER] = "ondragover", 
	[VHTMLATTR_ONDRAGSTART] = "ondragstart", [VHTMLATTR_ONDROP] = "ondrop", [VHTMLATTR_ONDURATIONCHANGE] = "ondurationchange", [VHTMLATTR_ONEMPTIED] = "onemptied",
	[VHTMLATTR_ONENDED] = "onended", [VHTMLATTR_ONERROR] = "onerror", [VHTMLATTR_ONFOCUS] = "onfocus", [VHTMLATTR_ONHASCHANGE] = "onhaschange", [VHTMLATTR_ONINPUT] = "oninput", 
	[VHTMLATTR_ONINVALID] = "oninvalid", [VHTMLATTR_ONKEYDOWN] = "onkeydown", [VHTMLATTR_ONKEYPRESS] = "onkeypress", [VHTMLATTR_ONKEYUP] = "onkeyup", 
	[VHTMLATTR_ONLOAD] = "onload", [VHTMLATTR_ONLOADEDDATA] = "onloadeddata", [VHTMLATTR_ONLOADEDMETADATA] = "onloadedmetadata", [VHTMLATTR_ONLOADSTART] = "onloadstart", 
	[VHTMLATTR_ONMOUSEDOWN] = "onmousedown", [VHTMLATTR_ONMOUSEMOVE] = "onmousemove", [VHTMLATTR_ONMOUSEOUT] = "onmouseout", [VHTMLATTR_ONMOUSEOVER] = "onmouseover", 
	[VHTMLATTR_ONMOUSEUP] = "onmouseup", [VHTMLATTR_ONMOUSEWHEEL] = "onmousewheel", [VHTMLATTR_ONOFFLINE] = "onoffline", [VHTMLATTR_ONONLINE] = "ononline", 
	[VHTMLATTR_ONPAGEHIDE] = "onpagehide", [VHTMLATTR_ONPAGESHOW] = "onpageshow", [VHTMLATTR_ONPASTE] = "onpaste", [VHTMLATTR_ONPAUSE] = "onpause", 
	[VHTMLATTR_ONPLAY] = "onplay", [VHTMLATTR_ONPLAYING] = "onplaying", [VHTMLATTR_ONPOPSTATE] = "onpopstate", [VHTMLATTR_ONPROGRESS] = "onprogress", 
	[VHTMLATTR_ONRATECHANGE] = "onratechange", [VHTMLATTR_ONRESET] = "onreset", [VHTMLATTR_ONRESIZE] = "onresize", [VHTMLATTR_ONSCROLL] = "onscroll", 
	[VHTMLATTR_ONSEARCH] = "onsearch", [VHTMLATTR_ONSEEKED] = "onseeked", [VHTMLATTR_ONSEEKING] = "onseeking", [VHTMLATTR_ONSELECT] = "onselect", 
	[VHTMLATTR_ONSTALLED] = "onstalled", [VHTMLATTR_ONSTORAGE] = "onstorage", [VHTMLATTR_ONSUBMIT] = "onsubmit", [VHTMLATTR_ONSUSPEND] = "onsuspend", 
	[VHTMLATTR_ONTIMEUPDATE] = "ontimeupdate", [VHTMLATTR_ONTOGGLE] = "ontoggle", [VHTMLATTR_ONUNLOAD] = "onunload", [VHTMLATTR_ONVOLUMECHANGE] = "onvolumechange", 
	[VHTMLATTR_ONWAITING] = "onwaiting", [VHTMLATTR_ONWHEEL] = "onwheel", [VHTMLATTR_OPEN] = "open", [VHTMLATTR_OPTIMUM] = "optimum", [VHTMLATTR_PATTERN] = "pattern", 
	[VHTMLATTR_PLACEHOLDER] = "placeholder", [VHTMLATTR_POPOVER] = "popover", [VHTMLATTR_POPOVERTARGET] = "popovertarget", [VHTMLATTR_POPOVERTARGETACTION] = "popovertargetaction", 
	[VHTMLATTR_POSTER] = "poster", [VHTMLATTR_PRELOAD] = "preload", [VHTMLATTR_READONLY] = "readonly", [VHTMLATTR_REL] = "rel", [VHTMLATTR_REQUIRED] = "required", 
	[VHTMLATTR_REVERSED] = "reversed", [VHTMLATTR_ROWS] = "rows", [VHTMLATTR_ROWSPAN] = "rowspan", [VHTMLATTR_SANDBOX] = "sandbox", [VHTMLATTR_SCOPE] = "scope", 
	[VHTMLATTR_SELECTED] = "selected", [VHTMLATTR_SHAPE] = "shape", [VHTMLATTR_SIZE] = "size", [VHTMLATTR_SIZES] = "sizes", [VHTMLATTR_SPAN] = "span", 
	[VHTMLATTR_SPELLCHECK] = "spellcheck", [VHTMLATTR_SRC] = "src", [VHTMLATTR_SRCDOC] = "srcdoc", [VHTMLATTR_SRCLANG] = "srclang", [VHTMLATTR_SRCSET] = "srcset", 
	[VHTMLATTR_START] = "start", [VHTMLATTR_STEP] = "step", [VHTMLATTR_STYLE] = "style", [VHTMLATTR_TABINDEX] = "tabindex", [VHTMLATTR_TARGET] = "target", [VHTMLATTR_TITLE] = "title", 
	[VHTMLATTR_TRANSLATE] = "translate", [VHTMLATTR_TYPE] = "type", [VHTMLATTR_USEMAP] = "usemap", [VHTMLATTR_VALUE] = "value", [VHTMLATTR_WIDTH] = "width", [VHTMLATTR_WRAP] = "wrap",
	[VHTMLATTR_XMLNS] = "xmlns", [VHTMLATTR_XMLNS_XLINK] = "xmlns:xlink"
};

const int void_tags[_VHTML_TAG_COUNT] = { //FIXME: Fill this out
	[VHTMLTAG_AREA] = 1, [VHTMLTAG_BASE] = 1, [VHTMLTAG_BR] = 1, [VHTMLTAG_COL] = 1,
	[VHTMLTAG_EMBED] = 1, [VHTMLTAG_HR] = 1, [VHTMLTAG_IMG] = 1, [VHTMLTAG_INPUT] = 1,
	[VHTMLTAG_LINK] = 1, [VHTMLTAG_META] = 1, [VHTMLTAG_PARAM] = 1, [VHTMLTAG_SOURCE] = 1,
	[VHTMLTAG_TRACK] = 1, [VHTMLTAG_WBR] = 1, [VHTMLTAG_DOCTYPE] = 1,
};

static inline size_t str_tolower_len(char *str) {
    size_t len = 0;
    for (unsigned char *p = (unsigned char *)str; *p; p++, len++)
        *p = (*p >= 'A' && *p <= 'Z') ? (*p | 0x20) : *p;
    return len;
}


VHTMLTag_t html_tag_from_str(char *str){
	size_t len = str_tolower_len(str);
	const struct VHTMLTag_st *tag = str ? lookup_htmltag(str, len) : NULL;
	return tag ? tag->ident : VHTMLTAG_CUSTOM;
}

VHTMLAttribute_t html_attr_from_str(char *str){
	size_t len = str_tolower_len(str);
	const struct VHTMLAttribute_st *tag = str ? lookup_htmlattr(str, len) : NULL;
	return tag ? tag->ident : _VHTML_ATTRIBUTE_COUNT;
}

VHTMLNode* create_element_t(VHTMLTag_t type, VHTMLArena *arena){
	VHTMLNode *e = arena_alloc(arena, sizeof(VHTMLNode), alignof(VHTMLNode));
	if(!e) return NULL;
	*e = (VHTMLNode){0};
	e->type = type;
	return e;
}

VHTMLNode* create_element(char *name, VHTMLArena *arena){
	return create_element_t(html_tag_from_str(name), arena);
}

// TODO:
/*

VHTMLDocument* create_document(){
	VHTMLDocument *doc = VHTML_CALLOC(1, sizeof(VHTMLDocument));
	if(!doc) return 0;
	VHTMLArena *arena = arena_init(100); // TODO too small
	doc->allocation = arena;
	doc->root = create_element_t(VHTMLTAG_HEAD, arena);
	return doc;
}
*/

VHTMLAttribute* set_attr(VHTMLArena *arena, VHTMLNode *node, VHTMLAttribute_t attribute, char *value){
	VHTMLAttribute* curr = node->attributes;
	if(curr){
		while(curr->type != attribute && curr->next) curr = curr->next;
		if(curr->type == attribute){
			curr->value = value;
			return curr;
		}
	}

	VHTMLAttribute* new_attr = arena_alloc(arena, sizeof(VHTMLAttribute), alignof(VHTMLAttribute));
	*new_attr = (VHTMLAttribute){0};
	new_attr->type = attribute;
	new_attr->value = value;

	if(!curr){
		node->attributes = new_attr;
		return new_attr;
	}
	
	curr->next = new_attr;

	return new_attr;
}

void unset_attr(VHTMLNode *node, VHTMLAttribute_t attribute){
	VHTMLAttribute *curr = node->attributes, *prev = 0;
	while(curr && curr->type != attribute){
		prev = curr;
		curr = curr->next;
	}
	if(!curr) return;
	if(prev) prev->next = curr->next; // since attributes are arena allocated, "freeing" them is unreferencing them
	return;
}


char *get_attr(VHTMLNode *node, VHTMLAttribute_t attribute){
	VHTMLAttribute* curr = node->attributes;
	while(curr && curr->type != attribute) curr = curr->next;
	if(!curr) return 0;
	return curr->value;
}



/* Parses the open tag of an html node, allocating attributes and their values to the provided
 * arena attached to the tag.
 *
 * @param open_lt_consumed - Signals to the parser that the first '<' that opens a tag was consumed in some
 * process before being handed the buffer (i.e. it became the null-terminator for some text belonging
 * to the parent) and the buffer points to some valid character after that '<'.
 * */
VHTMLNode* _parse_open_tag(char **buf, VHTMLArena *arena, int open_lt_consumed){
	char *next_char = *buf, curr_wid = 0, quote_type = 0, *resume = 0;
	char *curr_attr, *curr_value;
	VHTMLNode *node = NULL;
	VHTMLAttribute_t last_attr = _VHTML_ATTRIBUTE_COUNT;
	VHTMLDataAttribute *last_data_attr = 0;
	VHTMLNamespacedAttribute *last_ns_attr = 0;
	VHTMLUnknownAttribute *last_unk_attr = 0;
	int end_of_tag;

	enum STATES {
		PRE_TAG,
		TAG_OPEN,
		ATTR_NAME,
		ATTR_VALUE,
		TAG_END
	};

	enum STATES state = open_lt_consumed ? TAG_OPEN : PRE_TAG;
	enum STATES last_state = TAG_END;

parse:
	while(state != TAG_END){
		switch (state){
			case PRE_TAG:{
				last_state = PRE_TAG;
				// skip up to '<' and past
				char *lt = strchr(*buf, '<');
				if (!lt) { return node; } // no tag found
				*buf = lt + 1;
				state = TAG_OPEN;
				break;
			}
			case TAG_OPEN:{
				last_state = TAG_OPEN;
				char *tag = *buf; // tag name starts here, we null term at the end
				*buf += strcspn(*buf, " \t\n\r\f\v/>\0");
				if(**buf == '>' || **buf == '/') {
					state = TAG_END;
				}
				resume = next_char_utf8(*buf, 0);
				**buf = '\0'; // tag references start of tag, this null terms it and moves *buf
				node = create_element(tag, arena);
				*buf = resume;
				if(**buf == '>' || state == TAG_END) {
					state = TAG_END;
				} else {
					state = ATTR_NAME;
				}
				break;
			}
			case ATTR_NAME:
				last_state = ATTR_NAME;
				*buf += strspn(*buf, " \t\n\r\f\v");
				// we are here in 2 cases, A: a value was just assigned to an attribute and this is the start
				// of the next, or last attribute was a boolean attribute and there was no value assigned.
				// Because state ATTR_VALUE handles setting the attribute to a value if it exists, it also clears
				// last_attr to the sentinal _VHTML_ATTRIBUTE_COUNT. If it's not the sentinel, it was a bool attr
				// and that code was never run, so we check here and set the value as nessecary
				if(last_attr != _VHTML_ATTRIBUTE_COUNT){
					set_attr(arena, node, last_attr, 0);
					last_attr = _VHTML_ATTRIBUTE_COUNT;
				}
				if(last_data_attr){last_data_attr->value = 0; last_data_attr = 0;}
				if(last_ns_attr){last_ns_attr->value = 0; last_ns_attr = 0;}
				if(last_unk_attr) {last_unk_attr->value = 0; last_unk_attr = 0;}

				if (!**buf || **buf == '>') {state = TAG_END; break;}
				// the new attribute's data starts where buf was left
				curr_attr = *buf;

				// regardless of what this stops on, it's the end of the attribute name, so we cut short, 
				*buf += strcspn(*buf, " \t\n\r\f\v=>/\0");

				end_of_tag = (!*buf || **buf == '\0' || **buf == '>' || **buf == '/'); // end of tag
				int assignment = (**buf == '='); // could still be an assignment if false, may be whitespace

				char *resume_name = NULL;
				if (**buf) resume_name = next_char_utf8(*buf, 0);
				**buf = '\0'; // inplace nullterm of attribute name

				int xml;
				if((last_attr = html_attr_from_str(curr_attr)) != _VHTML_ATTRIBUTE_COUNT){
					//Good path
				} else if(strncmp(curr_attr, "data-", 5) == 0) { // custom data attribute
				// create the attribute, and set its name
					VHTMLDataAttribute *new_attr = arena_alloc(arena, sizeof(VHTMLDataAttribute), alignof(VHTMLDataAttribute));
					*new_attr = (VHTMLDataAttribute){0};
					VHTMLDataAttribute *da_tail = node->data_attrs;
					if(!da_tail){ // no custom exist so far
						node->data_attrs = new_attr;
					} else { // add at tail
						while(da_tail->next) da_tail = da_tail->next;
						da_tail->next = new_attr;
					}
					
					new_attr->name = curr_attr + 5; // cut out "data-" part
					last_data_attr = new_attr;
				} else if(
						(xml = !strncmp(curr_attr, "xml:", 4)) || // xml is a bool
						!strncmp(curr_attr, "xlink:", 6)){ // namespaced attribute
					VHTMLNamespacedAttribute *new_attr = arena_alloc(arena, sizeof(VHTMLNamespacedAttribute), alignof(VHTMLNamespacedAttribute));
					*new_attr = (VHTMLNamespacedAttribute){0};
					VHTMLNamespacedAttribute *ns_tail = node->ns_attrs;
					if(!ns_tail) {node->ns_attrs = new_attr;}
					else{
						while(ns_tail->next) ns_tail = ns_tail->next;
						ns_tail->next = new_attr;
					}
					
					new_attr->ns = xml ? VHTMLNS_XML : VHTMLNS_XLINK;

					new_attr->name = curr_attr + (xml ? 4 : 6); // cut out "xml"/"xlink" part
					last_ns_attr = new_attr;
				} else { // treat as a defined attribute
					VHTMLUnknownAttribute *new_attr = arena_alloc(arena, sizeof(VHTMLUnknownAttribute), alignof(VHTMLUnknownAttribute));
					*new_attr = (VHTMLUnknownAttribute){0};
					VHTMLUnknownAttribute *unk_tail = node->unk_attrs;
					if(!unk_tail) {node->unk_attrs = new_attr;}
					else{
						while(unk_tail->next) unk_tail = unk_tail->next;
						unk_tail->next = new_attr;
					}
					new_attr->name = curr_attr; // just anything is ok
					last_unk_attr = new_attr;
				}

				if (resume_name) {
					*buf = resume_name;
					*buf += strspn(*buf, " \t\n\r\f\v");
				}

				//if(!end_of_tag) (buf++); // set up for next iter
				if(!assignment) assignment = **buf && **buf == '=';
				if (assignment) {state = ATTR_VALUE;}
				else if (!*buf || **buf == '>' || **buf == '/') {state = TAG_END;}
				else {state = ATTR_NAME;}
				break;
			case ATTR_VALUE:
				last_state = ATTR_VALUE;
				// being here means **buf is pointing to an '=' character
				quote_type = 0;
				if(**buf == '=') *buf = next_char_utf8(*buf, &curr_wid);
				*buf += strspn(*buf, " \t\n\r\f\v");
				if(!**buf || **buf == '>' || **buf == '/'){
					state = ATTR_NAME; // handles boolean attributes
					break;
				}
				if(**buf == '"' || **buf == '\''){
					quote_type = **buf; // so we can match
					*buf = next_char_utf8(*buf, &curr_wid);
				}

				curr_value = *buf;

				if(quote_type){
					for(;;){
						char *end = strchr(*buf, quote_type);
						if (end) {
							if(*(end - 1) == '\\'){
								(*buf)++;
								continue;
							}
							curr_value = *buf;
							*buf = end;
							break;
						}
					}
				} else {
					*buf = strpbrk(*buf, " \t\n\r\f\v>\0");
				}

				end_of_tag = (!*buf || **buf == '>' || **buf == '/' || **buf == '\0'); // end of tag
				resume = next_char_utf8(*buf, 0);
				**buf = '\0'; // inplace nullterm of attribute name

				// put the value string in the appropriate attrib
				if(last_attr != _VHTML_ATTRIBUTE_COUNT) set_attr(arena, node, last_attr, curr_value)->quoted = quote_type;
				if(last_data_attr) last_data_attr->value = curr_value, last_data_attr->quoted = quote_type;
				if(last_ns_attr) last_ns_attr->value = curr_value, last_ns_attr->quoted = quote_type;
				if(last_unk_attr) last_unk_attr->value = curr_value, last_unk_attr->quoted = quote_type;
				last_attr = _VHTML_ATTRIBUTE_COUNT, last_data_attr = 0, last_ns_attr = 0, last_unk_attr = 0;

				if(end_of_tag){
					state = TAG_END;
					break;
				}
				*buf = resume;
				*buf += strspn(*buf, " \t\n\r\f\v");
				if (!*buf || **buf == '>' || **buf == '/' || **buf == '\0') { state = TAG_END; }
				else {state = ATTR_NAME;}
				break;
			default:
				state = TAG_END;
				break;
		}
	}
	if(*buf && **buf == '/'){
		if(!void_tags[node->type]){
			(*buf)++;
			state = last_state; // ignore it
			goto parse;
		}else{
			char *gt = strchr(*buf, '>');
			if(gt) *buf = gt;
		}
	}
	if(*buf && **buf == '>') (*buf)++; // return buf on first non tag open char
	return node;
}

static inline VHTMLNode *text_node(VHTMLArena *arena, char **buf, int *overwrote_open){
	char *next_tag = strchr(*buf, '<');
	if(next_tag == *buf) return NULL; // immediately is a new element
	VHTMLNode *node = create_element_t(_VHTMLTAG_TEXT, arena);
	node->_text = *buf;
	if(!next_tag){
		*buf += strlen(*buf);
	} else {
		*next_tag = '\0'; // nullterm
		*buf = next_tag;
		*overwrote_open = 1;
	}
	return node;	
}

// for parsing closing tags correctly

struct TAGSTACK {
	size_t size;
	long long top;
	VHTMLNode **stack;
};

static void st_push(struct TAGSTACK *stack, VHTMLNode *node){
	if(stack->top + 1 >= stack->size){
		stack->stack = VHTML_REALLOC(stack->stack, stack->size * 2 * sizeof(VHTMLNode*));// FIXME: handle fail
		stack->size *= 2;
	}
	stack->top++;
	stack->stack[stack->top] = node;
}

static VHTMLNode *st_pop(struct TAGSTACK *stack){
	if(stack->top >= 0){
		stack->top--;
		return stack->stack[stack->top + 1];
	}
	return NULL;
}

static VHTMLNode *st_peek(struct TAGSTACK *stack){
	return stack->top != -1 ? stack->stack[stack->top] : NULL;
}

// returns the type of the closed tag passed to it (on the first character of the tag name)
static inline VHTMLTag_t _parse_close_tag(char **buf){
	size_t name_len = strcspn(*buf, " \t\n\r\f\v>");
	char *start = *buf;
	int gt_immediate = 0;
	*buf += name_len;
	if(**buf == '>') gt_immediate = 1;
	**buf = '\0';
	if(!gt_immediate){
		char *close_gt = strchr(*buf, '>'); // skip until end
		if(close_gt) *buf = close_gt;
	} else {
		(*buf)++;
	}
	return html_tag_from_str(start);
}

/*
 * Destructively parses `buf` into a node allocated to `arena`
 *
 * @param parent - if parent is not NULL, parent becomes the stack sentinel
 *	and the first open tag is treated as the child of it
 * */
VHTMLNode* parse_node(VHTMLArena *arena, char *buf, VHTMLNode *parent){
    if(!arena) arena = arena_init(256);

    struct TAGSTACK tstack = {
        .size = 100,
        .top = -1,
        .stack = VHTML_CALLOC(100, sizeof(VHTMLNode*))
    };

	VHTMLNode *e = parent;

	if(!e){
		e = _parse_open_tag(&buf, arena, 0); // root parent
		if (!e) { VHTML_FREE(tstack.stack); return NULL; }
	}
	st_push(&tstack, e);

	int consumed_lt = 0; // if the < of the next open tag was made '\0' to nullterm

    while (*buf) {
        buf += strspn(buf, " \t\n\r\f\v"); // skip whitespace
        if (!*buf) break;
		VHTMLNode *next_child;

		if(consumed_lt){
			consumed_lt = 0;
			if (*buf == '/') { // consumed lt and is close tag
				buf += 1;
		        VHTMLTag_t type = _parse_close_tag(&buf);
			    if (st_peek(&tstack) && st_peek(&tstack)->type == type)
				    st_pop(&tstack);
				if (tstack.top < 0) break;
				consumed_lt = 0;
			    continue;
			} else { // normal open tag
				next_child = _parse_open_tag(&buf, arena, 1);
			}
		} else if (*buf == '<'){
			buf ++;
			if(*buf == '/'){
                buf++;
                VHTMLTag_t type = _parse_close_tag(&buf);
                if (st_peek(&tstack) && st_peek(&tstack)->type == type)
                    st_pop(&tstack);
                if (tstack.top < 0) break;
                continue;
			} else {
				next_child = _parse_open_tag(&buf, arena, 1);
			}
		} else {
			// text
			next_child = text_node(arena, &buf, &consumed_lt);
			if(consumed_lt) buf++;
		}

        if (!next_child) continue;

        VHTMLNode *parent = st_peek(&tstack);
        if (!parent) break;

        if (!parent->children) {
            parent->children = next_child;
            parent->_children_tail = next_child;
        } else {
            parent->_children_tail->forward_sibling = next_child;
            parent->_children_tail = next_child;
        }

		if(!void_tags[next_child->type] && next_child->type != _VHTMLTAG_TEXT) st_push(&tstack, next_child);
		#ifdef VHTML_DEBUG
			printf("Current stack:\n");
			for(int i = 0; i<tstack.top; i++){
				printf("\t%s,", html_tag_str[tstack.stack[i]->type]);
			}
			puts("");
			printf("\tnext_child: %s\n", html_tag_str[next_child->type]);
		#endif
    }

    VHTML_FREE(tstack.stack);
    return e;
}

/* 
 * Nondestructively parses a string into a DOM tree, returning a pointer to the heap allocated document struct.
 * It is important to note that if `arena` is NULL, the document will allocate an arena itsself of size FIXME bytes
 * which would need to be freed by caller to avoid leaks.
 * */
VHTMLDocument* document_from_string(VHTMLArena *arena, char *str){
	int owned_arena = !arena;
	if(owned_arena) arena = arena_init(4098);

	VHTMLDocument *doc = VHTML_CALLOC(1, sizeof(VHTMLDocument));
	if(!doc) goto cleanup;

	doc->allocation = arena;
	doc->root = create_element_t(_VHTMLTAG_ROOT, arena);
	doc->raw_buf = strdup(str);
	doc->owns_allocations = owned_arena;

	parse_node(arena, doc->raw_buf, doc->root);

	return doc;

cleanup:
	if(doc){
		if(doc->allocation) arena_free(doc->allocation);
		if(doc->raw_buf) VHTML_FREE(doc->raw_buf);
		if(doc->owns_allocations) arena_free(doc->allocation);
		VHTML_FREE(doc);
	}
	return NULL;
}

void free_document(VHTMLDocument *doc){
	if(doc){
		if(doc->raw_buf) VHTML_FREE(doc->raw_buf);
		VHTML_FREE(doc);
	}
}

#define P_ATTR(v, q, fmt, ...) do { \
    char _q[] = {(q), '\0'}; \
    if (format_style == VHTMLPRINT_DEBUG) fprintf(f, "\t" fmt ": %s\n", __VA_ARGS__, (v) ? (v) : ""); \
    else fprintf(f, " " fmt "%s%s%s%s", __VA_ARGS__, (v)?"=":"", (q)?_q:"", (v)?(v):"", (q)?_q:""); \
} while(0)

static void writeNode(FILE *f, VHTMLNode *node, enum VHTMLPrintFormat format_style, int depth) {
    if (!node) return;

    // text node
    if (node->type == _VHTMLTAG_TEXT) {
        if (format_style == VHTMLPRINT_DEBUG) fprintf(f, "%*sText: %s\n", depth * 2, "", node->_text ? node->_text : "");
        else {
			if(format_style == VHTMLPRINT_PRETTY) {for(int i = 0; i<depth; i++) fputs("  ", f);}
			fputs(node->_text ? node->_text : "", f);
			if(format_style == VHTMLPRINT_PRETTY) fputc('\n', f);
		}
        return;
    }

    if (format_style == VHTMLPRINT_DEBUG) fprintf(f, "%*sTag: %s\n", depth * 2, "", html_tag_str[node->type]);
    else {
		if(format_style == VHTMLPRINT_PRETTY){for(int i = 0; i<depth; i++) fputs("  ", f);}
		fprintf(f, "<%s", html_tag_str[node->type]);
	}

    for (VHTMLAttribute *c = node->attributes; c; c = c->next)
        P_ATTR(c->value, c->quoted, "%s", html_attribute_str[c->type]);
    for (VHTMLDataAttribute *c = node->data_attrs; c; c = c->next)
        P_ATTR(c->value, c->quoted, "data-%s", c->name);
    for (VHTMLNamespacedAttribute *c = node->ns_attrs; c; c = c->next)
        P_ATTR(c->value, c->quoted, "%s:%s",
               c->ns == VHTMLNS_XML ? "xml" : "xlink", c->name);
    for (VHTMLUnknownAttribute *c = node->unk_attrs; c; c = c->next)
        P_ATTR(c->value, c->quoted, "%s", c->name);

    if (format_style != VHTMLPRINT_DEBUG) fprintf(f, ">%s", format_style == VHTMLPRINT_PRETTY ? "\n" : "");

    // children
    for (VHTMLNode *ch = node->children; ch; ch = ch->forward_sibling)
        writeNode(f, ch, format_style, depth + 1);

    // close tag (no void)
    if ((format_style != VHTMLPRINT_DEBUG) && !void_tags[node->type]){
		if(format_style == VHTMLPRINT_PRETTY){for(int i = 0; i<depth; i++) fputs("  ", f);}
        fprintf(f, "</%s>%s", html_tag_str[node->type], format_style == VHTMLPRINT_PRETTY ? "\n" : "");
	}
}
#undef P_ATTR

char *serializeNode(VHTMLNode *node, enum VHTMLPrintFormat fmt) {
    char *buf = NULL;
    size_t size = 0;
    FILE *f = open_memstream(&buf, &size);
    if (!f) return NULL;

    writeNode(f, node, fmt, 0);

    fclose(f);
    return buf;
}

void printNode(VHTMLNode *node, enum VHTMLPrintFormat fmt) {
    char *str = serializeNode(node, fmt);
    if (!str) return;
    fputs(str, stdout);
    fputc('\n', stdout);
    free(str);
}

char *serializeDocument(VHTMLDocument *doc, enum VHTMLPrintFormat fmt){
    char *buf = NULL;
    size_t size = 0;
    FILE *f = open_memstream(&buf, &size);
    if (!f) return NULL;
	VHTMLNode *current = doc->root->children;

	while(current){
		writeNode(f, current, fmt, 0);
		current = current->forward_sibling;
	}

    fclose(f);
    return buf;
}
