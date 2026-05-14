#ifndef VHTML_H
#define VHTML_H

// ----------- Allocation -------------

// Choose your Own
#ifndef VHTML_ALLOC
	#include <stdlib.h>
	#define VHTML_MALLOC malloc
	#define VHTML_FREE free
	#define VHTML_CALLOC calloc
	#define VHTML_REALLOC realloc
#endif

struct VHTMLArenaChunk {
	unsigned char *data;
	size_t used;
	size_t size;
	struct VHTMLArenaChunk *next;
};

typedef struct VHTMLArena {
	struct VHTMLArenaChunk *head;
	struct VHTMLArenaChunk *tail;
} VHTMLArena;

VHTMLArena *arena_init(size_t size);
void arena_reset(VHTMLArena *arena);
void arena_free(VHTMLArena *arena);

// ----------- END: Allocation -------------

// 4/28/26
// _TYPE_COUNT should stay at the last element for indexing into
// https://html.spec.whatwg.org/multipage/indices.html#attributes-3
typedef enum {
	VHTMLTAG_DOCTYPE,
	VHTMLTAG_HTML, VHTMLTAG_BASE, VHTMLTAG_HEAD, VHTMLTAG_LINK, VHTMLTAG_META, VHTMLTAG_STYLE,
	VHTMLTAG_TITLE, VHTMLTAG_BODY, VHTMLTAG_ADDRESS, VHTMLTAG_ARTICLE, VHTMLTAG_ASIDE,
	VHTMLTAG_FOOTER, VHTMLTAG_HEADER, VHTMLTAG_H1, VHTMLTAG_H2, VHTMLTAG_H3, VHTMLTAG_H4,
	VHTMLTAG_H5, VHTMLTAG_H6, VHTMLTAG_HGROUP, VHTMLTAG_MAIN, VHTMLTAG_NAV, VHTMLTAG_SECTION,
	VHTMLTAG_SEARCH, VHTMLTAG_BLOCKQUOTE, VHTMLTAG_DD, VHTMLTAG_DIV, VHTMLTAG_DL,
	VHTMLTAG_DT, VHTMLTAG_FIGCAPTION, VHTMLTAG_FIGURE, VHTMLTAG_HR, VHTMLTAG_LI, VHTMLTAG_MENU,
	VHTMLTAG_OL, VHTMLTAG_P, VHTMLTAG_PRE, VHTMLTAG_UL, VHTMLTAG_A, VHTMLTAG_ABBR, VHTMLTAG_B, VHTMLTAG_BDI, VHTMLTAG_BDO,
	VHTMLTAG_BR, VHTMLTAG_CITE, VHTMLTAG_CODE, VHTMLTAG_DATA, VHTMLTAG_DFN, VHTMLTAG_EM, VHTMLTAG_I, VHTMLTAG_KBD,
	VHTMLTAG_MARK, VHTMLTAG_G, VHTMLTAG_RP, VHTMLTAG_RT, VHTMLTAG_RUBY, VHTMLTAG_S, VHTMLTAG_SAMP, VHTMLTAG_SMALL,
	VHTMLTAG_SPAN, VHTMLTAG_STRONG, VHTMLTAG_SUB, VHTMLTAG_SUP, VHTMLTAG_TIME, VHTMLTAG_U, VHTMLTAG_VAR, VHTMLTAG_WBR,
	VHTMLTAG_AREA, VHTMLTAG_AUDIO, VHTMLTAG_IMG, VHTMLTAG_MAP, VHTMLTAG_TRACK, VHTMLTAG_VIDEO,
	VHTMLTAG_EMBED, VHTMLTAG_FENCEDFRAME, VHTMLTAG_IFRAME, VHTMLTAG_OBJECT,
	VHTMLTAG_PICTURE, VHTMLTAG_SOURCE, VHTMLTAG_SVG, VHTMLTAG_MATH, VHTMLTAG_CANVAS, VHTMLTAG_NOSCRIPT,
	VHTMLTAG_SCRIPT, VHTMLTAG_DEL, VHTMLTAG_INS, VHTMLTAG_CAPTION, VHTMLTAG_COL, VHTMLTAG_COLGROUP,
	VHTMLTAG_TABLE, VHTMLTAG_TBODY, VHTMLTAG_TD, VHTMLTAG_TFOOT, VHTMLTAG_TH, VHTMLTAG_THEAD, VHTMLTAG_TR,
	VHTMLTAG_BUTTON, VHTMLTAG_DATALIST, VHTMLTAG_FIELDSET, VHTMLTAG_FORM, VHTMLTAG_INPUT,
	VHTMLTAG_LABEL, VHTMLTAG_LEGEND, VHTMLTAG_METER, VHTMLTAG_OPTGROUP, VHTMLTAG_OPTION,
	VHTMLTAG_OUTPUT, VHTMLTAG_PROGRESS, VHTMLTAG_SELECT, VHTMLTAG_SELECTEDCONTENT,
	VHTMLTAG_TEXTAREA, VHTMLTAG_DETAILS, VHTMLTAG_DIALOG, VHTMLTAG_GEOLOCATION,
	VHTMLTAG_SUMMARY, VHTMLTAG_SLOT, VHTMLTAG_TEMPLATE, VHTMLTAG_ACRONYM, VHTMLTAG_BIG, VHTMLTAG_CENTER,
	VHTMLTAG_CONTENT, VHTMLTAG_DIR, VHTMLTAG_FONT, VHTMLTAG_FRAME, VHTMLTAG_FRAMESET, VHTMLTAG_IMAGE,
	VHTMLTAG_MARQUEE, VHTMLTAG_MENUITEM, VHTMLTAG_NOBR, VHTMLTAG_NOEMBED, VHTMLTAG_NOFRAMES,
	VHTMLTAG_PARAM, VHTMLTAG_PLAINTEXT, VHTMLTAG_RB, VHTMLTAG_RTC, VHTMLTAG_SHADOW, VHTMLTAG_STRIKE, VHTMLTAG_TT,
	VHTMLTAG_XMP,
	
	VHTMLTAG_CUSTOM, _VHTMLTAG_TEXT,
	_VHTMLTAG_ROOT,
	_VHTML_TAG_COUNT
} VHTMLTag_t;

struct VHTMLTag_st {
	const char *tag_name;
	VHTMLTag_t ident;
};

struct VHTMLTag_st *lookup_htmltag (register const char *str, register size_t len);

typedef enum {
	VHTMLATTR_ACCEPT, VHTMLATTR_ACCEPTCHARSET, VHTMLATTR_ACCESSKEY, VHTMLATTR_ACTION, VHTMLATTR_ALIGN,
	VHTMLATTR_ALT, VHTMLATTR_ASYNC, VHTMLATTR_AUTOCOMPLETE, VHTMLATTR_AUTOFOCUS, VHTMLATTR_AUTOPLAY,
	VHTMLATTR_BGCOLOR, VHTMLATTR_BORDER, VHTMLATTR_CHARSET, VHTMLATTR_CHECKED, VHTMLATTR_CITE, VHTMLATTR_CLASS,
	VHTMLATTR_COLOR, VHTMLATTR_COLS, VHTMLATTR_COLSPAN, VHTMLATTR_CONTENT, VHTMLATTR_CONTENTEDITABLE,
	VHTMLATTR_CONTROLS, VHTMLATTR_COORDS, VHTMLATTR_DATA, VHTMLATTR_DATETIME, VHTMLATTR_DEFAULT, VHTMLATTR_DEFER,
	VHTMLATTR_DIR, VHTMLATTR_DIRNAME, VHTMLATTR_DISABLED, VHTMLATTR_DOWNLOAD, VHTMLATTR_DRAGGABLE, VHTMLATTR_ENCTYPE,
	VHTMLATTR_ENTERKEYHINT, VHTMLATTR_FOR, VHTMLATTR_FORM, VHTMLATTR_FORMACTION, VHTMLATTR_HEADERS, VHTMLATTR_HEIGHT,
	VHTMLATTR_HIDDEN, VHTMLATTR_HIGH, VHTMLATTR_HREF, VHTMLATTR_HREFLANG, VHTMLATTR_HTTPEQUIV, VHTMLATTR_ID, VHTMLATTR_INERT,
	VHTMLATTR_INPUTMODE, VHTMLATTR_ISMAP, VHTMLATTR_KIND, VHTMLATTR_LABEL, VHTMLATTR_LANG, VHTMLATTR_LIST, VHTMLATTR_LOOP, VHTMLATTR_LOW,
	VHTMLATTR_MAX, VHTMLATTR_MAXLENGTH, VHTMLATTR_MEDIA, VHTMLATTR_METHOD, VHTMLATTR_MIN, VHTMLATTR_MULTIPLE, VHTMLATTR_MUTED,
	VHTMLATTR_NAME, VHTMLATTR_NOVALIDATE, VHTMLATTR_ONABORT, VHTMLATTR_ONAFTERPRINT, VHTMLATTR_ONBEFOREPRINT,
	VHTMLATTR_ONBEFOREUNLOAD, VHTMLATTR_ONBLUR, VHTMLATTR_ONCANPLAY, VHTMLATTR_ONCANPLAYTHROUGH,
	VHTMLATTR_ONCHANGE, VHTMLATTR_ONCLICK, VHTMLATTR_ONCONTEXTMENU, VHTMLATTR_ONCOPY, VHTMLATTR_ONCUECHANGE,
	VHTMLATTR_ONCUT, VHTMLATTR_ONDBLCLICK, VHTMLATTR_ONDRAG, VHTMLATTR_ONDRAGEND, VHTMLATTR_ONDRAGENTER,
	VHTMLATTR_ONDRAGLEAVE, VHTMLATTR_ONDRAGOVER, VHTMLATTR_ONDRAGSTART, VHTMLATTR_ONDROP, VHTMLATTR_ONDURATIONCHANGE,
	VHTMLATTR_ONEMPTIED, VHTMLATTR_ONENDED, VHTMLATTR_ONERROR, VHTMLATTR_ONFOCUS, VHTMLATTR_ONHASCHANGE,
	VHTMLATTR_ONINPUT, VHTMLATTR_ONINVALID, VHTMLATTR_ONKEYDOWN, VHTMLATTR_ONKEYPRESS, VHTMLATTR_ONKEYUP, VHTMLATTR_ONLOAD,
	VHTMLATTR_ONLOADEDDATA, VHTMLATTR_ONLOADEDMETADATA, VHTMLATTR_ONLOADSTART, VHTMLATTR_ONMOUSEDOWN,
	VHTMLATTR_ONMOUSEMOVE, VHTMLATTR_ONMOUSEOUT, VHTMLATTR_ONMOUSEOVER, VHTMLATTR_ONMOUSEUP,
	VHTMLATTR_ONMOUSEWHEEL, VHTMLATTR_ONOFFLINE, VHTMLATTR_ONONLINE, VHTMLATTR_ONPAGEHIDE, VHTMLATTR_ONPAGESHOW,
	VHTMLATTR_ONPASTE, VHTMLATTR_ONPAUSE, VHTMLATTR_ONPLAY, VHTMLATTR_ONPLAYING, VHTMLATTR_ONPOPSTATE, VHTMLATTR_ONPROGRESS,
	VHTMLATTR_ONRATECHANGE, VHTMLATTR_ONRESET, VHTMLATTR_ONRESIZE, VHTMLATTR_ONSCROLL, VHTMLATTR_ONSEARCH, VHTMLATTR_ONSEEKED,
	VHTMLATTR_ONSEEKING, VHTMLATTR_ONSELECT, VHTMLATTR_ONSTALLED, VHTMLATTR_ONSTORAGE, VHTMLATTR_ONSUBMIT,
	VHTMLATTR_ONSUSPEND, VHTMLATTR_ONTIMEUPDATE, VHTMLATTR_ONTOGGLE, VHTMLATTR_ONUNLOAD, VHTMLATTR_ONVOLUMECHANGE,
	VHTMLATTR_ONWAITING, VHTMLATTR_ONWHEEL, VHTMLATTR_OPEN, VHTMLATTR_OPTIMUM, VHTMLATTR_PATTERN, VHTMLATTR_PLACEHOLDER, VHTMLATTR_POPOVER,
	VHTMLATTR_POPOVERTARGET, VHTMLATTR_POPOVERTARGETACTION, VHTMLATTR_POSTER, VHTMLATTR_PRELOAD, VHTMLATTR_READONLY, VHTMLATTR_REL,
	VHTMLATTR_REQUIRED, VHTMLATTR_REVERSED, VHTMLATTR_ROWS, VHTMLATTR_ROWSPAN, VHTMLATTR_SANDBOX, VHTMLATTR_SCOPE, VHTMLATTR_SELECTED,
	VHTMLATTR_SHAPE, VHTMLATTR_SIZE, VHTMLATTR_SIZES, VHTMLATTR_SPAN, VHTMLATTR_SPELLCHECK, VHTMLATTR_SRC, VHTMLATTR_SRCDOC, VHTMLATTR_SRCLANG,
	VHTMLATTR_SRCSET, VHTMLATTR_START, VHTMLATTR_STEP, VHTMLATTR_STYLE, VHTMLATTR_TABINDEX, VHTMLATTR_TARGET, VHTMLATTR_TITLE, VHTMLATTR_TRANSLATE,
	VHTMLATTR_TYPE, VHTMLATTR_USEMAP, VHTMLATTR_VALUE, VHTMLATTR_WIDTH, VHTMLATTR_WRAP,

	VHTMLATTR_XMLNS, VHTMLATTR_XMLNS_XLINK,

	VHTMLATTR_DATA_CUST, VHTMLATTR_HTML,
	_VHTML_ATTRIBUTE_COUNT
} VHTMLAttribute_t;

struct VHTMLAttribute_st {
	const char *attr_name;
	VHTMLAttribute_t ident;
};

struct VHTMLAttribute_st *lookup_htmlattr (register const char *str, register size_t len);

// 4 types of attributes, data- namespaced: defined, and unknown
typedef struct VHTMLDataAttribute {
	char *name;
	char *value;
	char quoted;
	struct VHTMLDataAttribute *next;
} VHTMLDataAttribute;

typedef enum VHTMLNamespace {
	VHTMLNS_XML,
	VHTMLNS_XLINK
} VHTMLNamespace;

typedef struct VHTMLNamespacedAttribute {
	VHTMLNamespace ns;
	char *name;
	char quoted;
	char *value;
	struct VHTMLNamespacedAttribute *next;
} VHTMLNamespacedAttribute;

typedef struct VHTMLAttribute {
	VHTMLAttribute_t type;
	char quoted;
	char *value;
	struct VHTMLAttribute *next;
} VHTMLAttribute;

typedef struct VHTMLUnknownAttribute {
	char *name;
	char quoted;
	char *value;
	struct VHTMLUnknownAttribute *next;
} VHTMLUnknownAttribute;

// ------ END: Attributes -------

typedef struct VHTMLNode {
	VHTMLTag_t type;
	struct VHTMLNode *children;
	struct VHTMLNode *last_child;
	struct VHTMLNode *parent;
	struct VHTMLAttribute *attributes;
	struct VHTMLDataAttribute *data_attrs;
	struct VHTMLNamespacedAttribute *ns_attrs;
	struct VHTMLUnknownAttribute *unk_attrs;
	char *_text;
	struct VHTMLNode *forward_sibling;
} VHTMLNode;

typedef struct VHTMLDocument {
	VHTMLNode *root;
	VHTMLArena *allocation;
	char *raw_buf;
	int owns_allocations;
	VHTMLNode *body;
} VHTMLDocument;

extern const char *const html_element_str[];
extern const char *const html_attribute_str[];

VHTMLNode* create_element_t(VHTMLTag_t type, VHTMLArena *arena);
VHTMLNode* create_element(char *name, VHTMLArena *arena);
VHTMLNode* parse_node(VHTMLArena *arena, char *buf, VHTMLNode *parent);

// Sets an attribute to some heap/arena allocated value for some node, 0 implying a
// boolean attribute's existence. Thus, it may be undone with the unset_attr function
VHTMLAttribute* set_attr(VHTMLArena *arena, VHTMLNode *node, VHTMLAttribute_t attribute, char *value);
void unset_attr(VHTMLNode *node, VHTMLAttribute_t attribute);
char* get_attr(VHTMLNode *node, VHTMLAttribute_t attribute);

VHTMLDocument* create_document(VHTMLArena *arena, char *str);
void free_document(VHTMLDocument *doc);

enum VHTMLPrintFormat {
	VHTMLPRINT_DEBUG,
	VHTMLPRINT_PRETTY,
	VHTMLPRINT_COMPACT
};

void printNode(VHTMLNode *node, enum VHTMLPrintFormat fmt);
char *serializeNode(VHTMLNode *node, enum VHTMLPrintFormat fmt);


VHTMLNode *append_child(VHTMLNode *parent, VHTMLNode *child);
VHTMLNode *set_inner_html(VHTMLArena *arena, VHTMLNode *parent, char *str);
VHTMLNode *set_inner_text(VHTMLArena *arena, VHTMLNode *parent, char *str);
char *get_text_content(VHTMLNode *node, VHTMLArena *arena);

char *serializeDocument(VHTMLDocument *doc, enum VHTMLPrintFormat fmt);
#endif
#ifdef VHTML_IMPLEMENTATION
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf tags.gperf  */
/* Computed positions: -k'1-2,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "tags.gperf"

#include <string.h>
#ifndef VHTML_H
    #include "../../vhtml_h.h"
#endif
#line 7 "tags.gperf"
struct VHTMLTag_st;
#include <string.h>

#define TOTAL_KEYWORDS 140
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 345
/* maximum key range = 345, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
html_tag_hash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346,  30, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346,  55,
       35,  35,   5,   5,   0,   5,   0,   0, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346,  80,  30, 140,
       10,  15,  45,  10,   0,  90,  10, 130,   0,  35,
       15, 105,  25,  10,  25,   0,   5,  55, 165, 170,
      155,  70, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346, 346,
      346, 346, 346, 346, 346, 346, 346, 346, 346
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[1]+3];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct VHTMLTag_st *
lookup_htmltag (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct VHTMLTag_st html_tag_pool[] =
    {
      {""},
#line 76 "tags.gperf"
      {"s", VHTMLTAG_S},
#line 38 "tags.gperf"
      {"h6", VHTMLTAG_H6},
      {""}, {""}, {""},
#line 43 "tags.gperf"
      {"search", VHTMLTAG_SEARCH},
#line 37 "tags.gperf"
      {"h5", VHTMLTAG_H5},
      {""}, {""}, {""},
#line 128 "tags.gperf"
      {"select", VHTMLTAG_SELECT},
#line 36 "tags.gperf"
      {"h4", VHTMLTAG_H4},
#line 104 "tags.gperf"
      {"del", VHTMLTAG_DEL},
#line 22 "tags.gperf"
      {"head", VHTMLTAG_HEAD},
#line 121 "tags.gperf"
      {"label", VHTMLTAG_LABEL},
#line 122 "tags.gperf"
      {"legend", VHTMLTAG_LEGEND},
#line 131 "tags.gperf"
      {"details", VHTMLTAG_DETAILS},
      {""},
#line 79 "tags.gperf"
      {"span", VHTMLTAG_SPAN},
#line 129 "tags.gperf"
      {"selectedcontent", VHTMLTAG_SELECTEDCONTENT},
#line 72 "tags.gperf"
      {"g", VHTMLTAG_G},
#line 42 "tags.gperf"
      {"section", VHTMLTAG_SECTION},
      {""},
#line 83 "tags.gperf"
      {"time", VHTMLTAG_TIME},
#line 26 "tags.gperf"
      {"title", VHTMLTAG_TITLE},
#line 132 "tags.gperf"
      {"dialog", VHTMLTAG_DIALOG},
#line 111 "tags.gperf"
      {"td", VHTMLTAG_TD},
#line 136 "tags.gperf"
      {"template", VHTMLTAG_TEMPLATE},
      {""},
#line 78 "tags.gperf"
      {"small", VHTMLTAG_SMALL},
#line 32 "tags.gperf"
      {"header", VHTMLTAG_HEADER},
#line 45 "tags.gperf"
      {"dd", VHTMLTAG_DD},
#line 117 "tags.gperf"
      {"datalist", VHTMLTAG_DATALIST},
      {""},
#line 109 "tags.gperf"
      {"table", VHTMLTAG_TABLE},
#line 133 "tags.gperf"
      {"geolocation", VHTMLTAG_GEOLOCATION},
#line 35 "tags.gperf"
      {"h3", VHTMLTAG_H3},
#line 141 "tags.gperf"
      {"dir", VHTMLTAG_DIR},
#line 77 "tags.gperf"
      {"samp", VHTMLTAG_SAMP},
      {""},
#line 39 "tags.gperf"
      {"hgroup", VHTMLTAG_HGROUP},
#line 34 "tags.gperf"
      {"h2", VHTMLTAG_H2},
#line 138 "tags.gperf"
      {"big", VHTMLTAG_BIG},
      {""}, {""},
#line 98 "tags.gperf"
      {"source", VHTMLTAG_SOURCE},
#line 97 "tags.gperf"
      {"picture", VHTMLTAG_PICTURE},
#line 150 "tags.gperf"
      {"noframes", VHTMLTAG_NOFRAMES},
#line 100 "tags.gperf"
      {"math", VHTMLTAG_MATH},
      {""},
#line 55 "tags.gperf"
      {"p", VHTMLTAG_P},
#line 73 "tags.gperf"
      {"rp", VHTMLTAG_RP},
#line 102 "tags.gperf"
      {"noscript", VHTMLTAG_NOSCRIPT},
      {""},
#line 93 "tags.gperf"
      {"embed", VHTMLTAG_EMBED},
#line 103 "tags.gperf"
      {"script", VHTMLTAG_SCRIPT},
#line 149 "tags.gperf"
      {"noembed", VHTMLTAG_NOEMBED},
#line 118 "tags.gperf"
      {"fieldset", VHTMLTAG_FIELDSET},
#line 21 "tags.gperf"
      {"base", VHTMLTAG_BASE},
      {""},
#line 60 "tags.gperf"
      {"b", VHTMLTAG_B},
#line 33 "tags.gperf"
      {"h1", VHTMLTAG_H1},
#line 19 "tags.gperf"
      {"!doctype", VHTMLTAG_DOCTYPE},
#line 40 "tags.gperf"
      {"main", VHTMLTAG_MAIN},
#line 123 "tags.gperf"
      {"meter", VHTMLTAG_METER},
#line 50 "tags.gperf"
      {"figure", VHTMLTAG_FIGURE},
#line 146 "tags.gperf"
      {"marquee", VHTMLTAG_MARQUEE},
      {""},
#line 148 "tags.gperf"
      {"nobr", VHTMLTAG_NOBR},
#line 49 "tags.gperf"
      {"figcaption", VHTMLTAG_FIGCAPTION},
#line 94 "tags.gperf"
      {"fencedframe", VHTMLTAG_FENCEDFRAME},
#line 153 "tags.gperf"
      {"rb", VHTMLTAG_RB},
#line 90 "tags.gperf"
      {"map", VHTMLTAG_MAP},
      {""},
#line 151 "tags.gperf"
      {"param", VHTMLTAG_PARAM},
      {""},
#line 68 "tags.gperf"
      {"em", VHTMLTAG_EM},
#line 147 "tags.gperf"
      {"menuitem", VHTMLTAG_MENUITEM},
#line 142 "tags.gperf"
      {"font", VHTMLTAG_FONT},
      {""}, {""},
#line 51 "tags.gperf"
      {"hr", VHTMLTAG_HR},
#line 99 "tags.gperf"
      {"svg", VHTMLTAG_SVG},
      {""}, {""}, {""},
#line 115 "tags.gperf"
      {"tr", VHTMLTAG_TR},
#line 127 "tags.gperf"
      {"progress", VHTMLTAG_PROGRESS},
      {""}, {""}, {""},
#line 52 "tags.gperf"
      {"li", VHTMLTAG_LI},
#line 130 "tags.gperf"
      {"textarea", VHTMLTAG_TEXTAREA},
#line 53 "tags.gperf"
      {"menu", VHTMLTAG_MENU},
#line 110 "tags.gperf"
      {"tbody", VHTMLTAG_TBODY},
      {""},
#line 28 "tags.gperf"
      {"address", VHTMLTAG_ADDRESS},
#line 56 "tags.gperf"
      {"pre", VHTMLTAG_PRE},
      {""}, {""},
#line 31 "tags.gperf"
      {"footer", VHTMLTAG_FOOTER},
      {""},
#line 105 "tags.gperf"
      {"ins", VHTMLTAG_INS},
#line 66 "tags.gperf"
      {"data", VHTMLTAG_DATA},
#line 112 "tags.gperf"
      {"tfoot", VHTMLTAG_TFOOT},
      {""}, {""}, {""},
#line 119 "tags.gperf"
      {"form", VHTMLTAG_FORM},
#line 120 "tags.gperf"
      {"input", VHTMLTAG_INPUT},
#line 84 "tags.gperf"
      {"u", VHTMLTAG_U},
#line 63 "tags.gperf"
      {"br", VHTMLTAG_BR},
#line 144 "tags.gperf"
      {"frameset", VHTMLTAG_FRAMESET},
#line 135 "tags.gperf"
      {"slot", VHTMLTAG_SLOT},
      {""}, {""},
#line 47 "tags.gperf"
      {"dl", VHTMLTAG_DL},
#line 67 "tags.gperf"
      {"dfn", VHTMLTAG_DFN},
#line 24 "tags.gperf"
      {"meta", VHTMLTAG_META},
#line 143 "tags.gperf"
      {"frame", VHTMLTAG_FRAME},
      {""}, {""}, {""},
#line 59 "tags.gperf"
      {"abbr", VHTMLTAG_ABBR},
      {""},
#line 125 "tags.gperf"
      {"option", VHTMLTAG_OPTION},
      {""},
#line 89 "tags.gperf"
      {"img", VHTMLTAG_IMG},
#line 27 "tags.gperf"
      {"body", VHTMLTAG_BODY},
      {""},
#line 96 "tags.gperf"
      {"object", VHTMLTAG_OBJECT},
      {""},
#line 61 "tags.gperf"
      {"bdi", VHTMLTAG_BDI},
#line 23 "tags.gperf"
      {"link", VHTMLTAG_LINK},
#line 145 "tags.gperf"
      {"image", VHTMLTAG_IMAGE},
      {""},
#line 113 "tags.gperf"
      {"th", VHTMLTAG_TH},
#line 124 "tags.gperf"
      {"optgroup", VHTMLTAG_OPTGROUP},
      {""}, {""}, {""}, {""}, {""},
#line 152 "tags.gperf"
      {"plaintext", VHTMLTAG_PLAINTEXT},
      {""}, {""}, {""},
#line 62 "tags.gperf"
      {"bdo", VHTMLTAG_BDO},
      {""},
#line 114 "tags.gperf"
      {"thead", VHTMLTAG_THEAD},
      {""}, {""}, {""}, {""}, {""},
#line 101 "tags.gperf"
      {"canvas", VHTMLTAG_CANVAS},
#line 29 "tags.gperf"
      {"article", VHTMLTAG_ARTICLE},
#line 70 "tags.gperf"
      {"kbd", VHTMLTAG_KBD},
#line 64 "tags.gperf"
      {"cite", VHTMLTAG_CITE},
#line 44 "tags.gperf"
      {"blockquote", VHTMLTAG_BLOCKQUOTE},
#line 58 "tags.gperf"
      {"a", VHTMLTAG_A},
#line 57 "tags.gperf"
      {"ul", VHTMLTAG_UL},
      {""}, {""}, {""}, {""},
#line 137 "tags.gperf"
      {"acronym", VHTMLTAG_ACRONYM},
#line 107 "tags.gperf"
      {"col", VHTMLTAG_COL},
      {""}, {""},
#line 139 "tags.gperf"
      {"center", VHTMLTAG_CENTER},
#line 106 "tags.gperf"
      {"caption", VHTMLTAG_CAPTION},
      {""},
#line 20 "tags.gperf"
      {"html", VHTMLTAG_HTML},
      {""}, {""},
#line 140 "tags.gperf"
      {"content", VHTMLTAG_CONTENT},
#line 46 "tags.gperf"
      {"div", VHTMLTAG_DIV},
#line 71 "tags.gperf"
      {"mark", VHTMLTAG_MARK},
      {""},
#line 69 "tags.gperf"
      {"i", VHTMLTAG_I},
#line 157 "tags.gperf"
      {"tt", VHTMLTAG_TT},
#line 82 "tags.gperf"
      {"sup", VHTMLTAG_SUP},
#line 65 "tags.gperf"
      {"code", VHTMLTAG_CODE},
      {""},
#line 80 "tags.gperf"
      {"strong", VHTMLTAG_STRONG},
#line 48 "tags.gperf"
      {"dt", VHTMLTAG_DT},
#line 81 "tags.gperf"
      {"sub", VHTMLTAG_SUB},
      {""},
#line 25 "tags.gperf"
      {"style", VHTMLTAG_STYLE},
#line 156 "tags.gperf"
      {"strike", VHTMLTAG_STRIKE},
      {""},
#line 41 "tags.gperf"
      {"nav", VHTMLTAG_NAV},
      {""},
#line 91 "tags.gperf"
      {"track", VHTMLTAG_TRACK},
      {""}, {""},
#line 108 "tags.gperf"
      {"colgroup", VHTMLTAG_COLGROUP},
      {""}, {""},
#line 95 "tags.gperf"
      {"iframe", VHTMLTAG_IFRAME},
#line 74 "tags.gperf"
      {"rt", VHTMLTAG_RT},
#line 85 "tags.gperf"
      {"var", VHTMLTAG_VAR},
      {""}, {""},
#line 116 "tags.gperf"
      {"button", VHTMLTAG_BUTTON},
      {""},
#line 158 "tags.gperf"
      {"xmp", VHTMLTAG_XMP},
      {""}, {""}, {""},
#line 54 "tags.gperf"
      {"ol", VHTMLTAG_OL},
#line 86 "tags.gperf"
      {"wbr", VHTMLTAG_WBR},
      {""}, {""}, {""}, {""}, {""},
#line 87 "tags.gperf"
      {"area", VHTMLTAG_AREA},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 134 "tags.gperf"
      {"summary", VHTMLTAG_SUMMARY},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 75 "tags.gperf"
      {"ruby", VHTMLTAG_RUBY},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 30 "tags.gperf"
      {"aside", VHTMLTAG_ASIDE},
      {""}, {""}, {""}, {""}, {""},
#line 126 "tags.gperf"
      {"output", VHTMLTAG_OUTPUT},
      {""}, {""}, {""},
#line 92 "tags.gperf"
      {"video", VHTMLTAG_VIDEO},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 155 "tags.gperf"
      {"shadow", VHTMLTAG_SHADOW},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 154 "tags.gperf"
      {"rtc", VHTMLTAG_RTC},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 88 "tags.gperf"
      {"audio", VHTMLTAG_AUDIO}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = html_tag_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = html_tag_pool[key].tag_name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &html_tag_pool[key];
        }
    }
  return (struct VHTMLTag_st *) 0;
}
#line 159 "tags.gperf"

#undef TOTAL_KEYWORDS
#undef MIN_WORD_LENGTH
#undef MAX_WORD_LENGTH
#undef MIN_HASH_VALUE
#undef MAX_HASH_VALUE
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf attributes.gperf  */
/* Computed positions: -k'1,3,5,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "attributes.gperf"

#include <string.h>
#ifndef VHTML_H
    #include "../../vhtml_h.h"
#endif
#line 7 "attributes.gperf"
struct VHTMLAttribute_st;
#include <string.h>

#define TOTAL_KEYWORDS 175
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 19
#define MIN_HASH_VALUE 12
#define MAX_HASH_VALUE 489
/* maximum key range = 478, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
html_attribute_hash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490,  20, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490,   0, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490,  15, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490,  40,  75,  50,
        5,  10, 150,  70, 165,   5, 490, 200,  60,   5,
      140,   0,  80,   5, 110,  25,   5, 100, 130,  80,
       95, 140,  45, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490, 490, 490, 490, 490,
      490, 490, 490, 490, 490, 490
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[4]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct VHTMLAttribute_st *
lookup_htmlattr (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct VHTMLAttribute_st html_attribute_pool[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 64 "attributes.gperf"
      {"id", VHTMLATTR_ID},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 77 "attributes.gperf"
      {"method", VHTMLATTR_METHOD},
#line 154 "attributes.gperf"
      {"optimum", VHTMLATTR_OPTIMUM},
      {""}, {""},
#line 80 "attributes.gperf"
      {"muted", VHTMLATTR_MUTED},
      {""},
#line 106 "attributes.gperf"
      {"onended", VHTMLATTR_ONENDED},
      {""}, {""},
#line 65 "attributes.gperf"
      {"inert", VHTMLATTR_INERT},
      {""},
#line 147 "attributes.gperf"
      {"ontimeupdate", VHTMLATTR_ONTIMEUPDATE},
#line 44 "attributes.gperf"
      {"datetime", VHTMLATTR_DATETIME},
      {""},
#line 185 "attributes.gperf"
      {"title", VHTMLATTR_TITLE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 144 "attributes.gperf"
      {"onstorage", VHTMLATTR_ONSTORAGE},
      {""}, {""}, {""},
#line 140 "attributes.gperf"
      {"onseeked", VHTMLATTR_ONSEEKED},
      {""},
#line 169 "attributes.gperf"
      {"scope", VHTMLATTR_SCOPE},
      {""},
#line 83 "attributes.gperf"
      {"onabort", VHTMLATTR_ONABORT},
#line 25 "attributes.gperf"
      {"alt", VHTMLATTR_ALT},
#line 43 "attributes.gperf"
      {"data", VHTMLATTR_DATA},
      {""}, {""}, {""}, {""},
#line 98 "attributes.gperf"
      {"ondragend", VHTMLATTR_ONDRAGEND},
      {""},
#line 102 "attributes.gperf"
      {"ondragstart", VHTMLATTR_ONDRAGSTART},
#line 84 "attributes.gperf"
      {"onafterprint", VHTMLATTR_ONAFTERPRINT},
      {""},
#line 146 "attributes.gperf"
      {"onsuspend", VHTMLATTR_ONSUSPEND},
#line 95 "attributes.gperf"
      {"oncut", VHTMLATTR_ONCUT},
#line 100 "attributes.gperf"
      {"ondragleave", VHTMLATTR_ONDRAGLEAVE},
      {""}, {""},
#line 34 "attributes.gperf"
      {"cite", VHTMLATTR_CITE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 126 "attributes.gperf"
      {"ononline", VHTMLATTR_ONONLINE},
#line 143 "attributes.gperf"
      {"onstalled", VHTMLATTR_ONSTALLED},
#line 180 "attributes.gperf"
      {"start", VHTMLATTR_START},
#line 94 "attributes.gperf"
      {"oncuechange", VHTMLATTR_ONCUECHANGE},
      {""}, {""},
#line 172 "attributes.gperf"
      {"size", VHTMLATTR_SIZE},
      {""},
#line 42 "attributes.gperf"
      {"coords", VHTMLATTR_COORDS},
      {""},
#line 79 "attributes.gperf"
      {"multiple", VHTMLATTR_MULTIPLE},
#line 186 "attributes.gperf"
      {"translate", VHTMLATTR_TRANSLATE},
#line 171 "attributes.gperf"
      {"shape", VHTMLATTR_SHAPE},
#line 103 "attributes.gperf"
      {"ondrop", VHTMLATTR_ONDROP},
      {""},
#line 148 "attributes.gperf"
      {"ontoggle", VHTMLATTR_ONTOGGLE},
#line 71 "attributes.gperf"
      {"list", VHTMLATTR_LIST},
#line 76 "attributes.gperf"
      {"media", VHTMLATTR_MEDIA},
#line 179 "attributes.gperf"
      {"srcset", VHTMLATTR_SRCSET},
#line 110 "attributes.gperf"
      {"oninput", VHTMLATTR_ONINPUT},
#line 142 "attributes.gperf"
      {"onselect", VHTMLATTR_ONSELECT},
#line 187 "attributes.gperf"
      {"type", VHTMLATTR_TYPE},
      {""}, {""},
#line 161 "attributes.gperf"
      {"preload", VHTMLATTR_PRELOAD},
      {""},
#line 105 "attributes.gperf"
      {"onemptied", VHTMLATTR_ONEMPTIED},
      {""}, {""}, {""},
#line 90 "attributes.gperf"
      {"onchange", VHTMLATTR_ONCHANGE},
#line 66 "attributes.gperf"
      {"inputmode", VHTMLATTR_INPUTMODE},
      {""},
#line 115 "attributes.gperf"
      {"onload", VHTMLATTR_ONLOAD},
      {""},
#line 145 "attributes.gperf"
      {"onsubmit", VHTMLATTR_ONSUBMIT},
#line 141 "attributes.gperf"
      {"onseeking", VHTMLATTR_ONSEEKING},
#line 134 "attributes.gperf"
      {"onprogress", VHTMLATTR_ONPROGRESS},
#line 118 "attributes.gperf"
      {"onloadstart", VHTMLATTR_ONLOADSTART},
#line 27 "attributes.gperf"
      {"autocomplete", VHTMLATTR_AUTOCOMPLETE},
#line 49 "attributes.gperf"
      {"disabled", VHTMLATTR_DISABLED},
#line 181 "attributes.gperf"
      {"step", VHTMLATTR_STEP},
#line 121 "attributes.gperf"
      {"onmouseout", VHTMLATTR_ONMOUSEOUT},
#line 97 "attributes.gperf"
      {"ondrag", VHTMLATTR_ONDRAG},
#line 129 "attributes.gperf"
      {"onpaste", VHTMLATTR_ONPASTE},
      {""}, {""},
#line 173 "attributes.gperf"
      {"sizes", VHTMLATTR_SIZES},
#line 120 "attributes.gperf"
      {"onmousemove", VHTMLATTR_ONMOUSEMOVE},
#line 32 "attributes.gperf"
      {"charset", VHTMLATTR_CHARSET},
#line 176 "attributes.gperf"
      {"src", VHTMLATTR_SRC},
      {""}, {""},
#line 177 "attributes.gperf"
      {"srcdoc", VHTMLATTR_SRCDOC},
      {""},
#line 164 "attributes.gperf"
      {"required", VHTMLATTR_REQUIRED},
#line 51 "attributes.gperf"
      {"draggable", VHTMLATTR_DRAGGABLE},
      {""},
#line 184 "attributes.gperf"
      {"target", VHTMLATTR_TARGET},
#line 135 "attributes.gperf"
      {"onratechange", VHTMLATTR_ONRATECHANGE},
      {""},
#line 37 "attributes.gperf"
      {"cols", VHTMLATTR_COLS},
      {""},
#line 104 "attributes.gperf"
      {"ondurationchange", VHTMLATTR_ONDURATIONCHANGE},
#line 53 "attributes.gperf"
      {"enterkeyhint", VHTMLATTR_ENTERKEYHINT},
      {""},
#line 72 "attributes.gperf"
      {"loop", VHTMLATTR_LOOP},
#line 35 "attributes.gperf"
      {"class", VHTMLATTR_CLASS},
      {""},
#line 136 "attributes.gperf"
      {"onreset", VHTMLATTR_ONRESET},
#line 170 "attributes.gperf"
      {"selected", VHTMLATTR_SELECTED},
#line 111 "attributes.gperf"
      {"oninvalid", VHTMLATTR_ONINVALID},
      {""}, {""},
#line 116 "attributes.gperf"
      {"onloadeddata", VHTMLATTR_ONLOADEDDATA},
#line 137 "attributes.gperf"
      {"onresize", VHTMLATTR_ONRESIZE},
#line 153 "attributes.gperf"
      {"open", VHTMLATTR_OPEN},
      {""},
#line 117 "attributes.gperf"
      {"onloadedmetadata", VHTMLATTR_ONLOADEDMETADATA},
#line 152 "attributes.gperf"
      {"onwheel", VHTMLATTR_ONWHEEL},
#line 50 "attributes.gperf"
      {"download", VHTMLATTR_DOWNLOAD},
#line 81 "attributes.gperf"
      {"name", VHTMLATTR_NAME},
      {""}, {""}, {""}, {""},
#line 151 "attributes.gperf"
      {"onwaiting", VHTMLATTR_ONWAITING},
#line 101 "attributes.gperf"
      {"ondragover", VHTMLATTR_ONDRAGOVER},
#line 99 "attributes.gperf"
      {"ondragenter", VHTMLATTR_ONDRAGENTER},
      {""}, {""},
#line 125 "attributes.gperf"
      {"onoffline", VHTMLATTR_ONOFFLINE},
#line 127 "attributes.gperf"
      {"onpagehide", VHTMLATTR_ONPAGEHIDE},
      {""},
#line 48 "attributes.gperf"
      {"dirname", VHTMLATTR_DIRNAME},
#line 149 "attributes.gperf"
      {"onunload", VHTMLATTR_ONUNLOAD},
      {""},
#line 67 "attributes.gperf"
      {"ismap", VHTMLATTR_ISMAP},
      {""},
#line 124 "attributes.gperf"
      {"onmousewheel", VHTMLATTR_ONMOUSEWHEEL},
      {""}, {""},
#line 133 "attributes.gperf"
      {"onpopstate", VHTMLATTR_ONPOPSTATE},
#line 20 "attributes.gperf"
      {"accept", VHTMLATTR_ACCEPT},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 21 "attributes.gperf"
      {"accept-charset", VHTMLATTR_ACCEPTCHARSET},
#line 182 "attributes.gperf"
      {"style", VHTMLATTR_STYLE},
#line 23 "attributes.gperf"
      {"action", VHTMLATTR_ACTION},
#line 178 "attributes.gperf"
      {"srclang", VHTMLATTR_SRCLANG},
      {""},
#line 123 "attributes.gperf"
      {"onmouseup", VHTMLATTR_ONMOUSEUP},
      {""}, {""},
#line 130 "attributes.gperf"
      {"onpause", VHTMLATTR_ONPAUSE},
#line 74 "attributes.gperf"
      {"max", VHTMLATTR_MAX},
#line 132 "attributes.gperf"
      {"onplaying", VHTMLATTR_ONPLAYING},
      {""}, {""}, {""},
#line 138 "attributes.gperf"
      {"onscroll", VHTMLATTR_ONSCROLL},
#line 191 "attributes.gperf"
      {"wrap", VHTMLATTR_WRAP},
      {""}, {""}, {""}, {""},
#line 174 "attributes.gperf"
      {"span", VHTMLATTR_SPAN},
#line 192 "attributes.gperf"
      {"xmlns", VHTMLATTR_XMLNS},
#line 109 "attributes.gperf"
      {"onhaschange", VHTMLATTR_ONHASCHANGE},
#line 39 "attributes.gperf"
      {"content", VHTMLATTR_CONTENT},
      {""},
#line 150 "attributes.gperf"
      {"onvolumechange", VHTMLATTR_ONVOLUMECHANGE},
#line 189 "attributes.gperf"
      {"value", VHTMLATTR_VALUE},
      {""},
#line 52 "attributes.gperf"
      {"enctype", VHTMLATTR_ENCTYPE},
      {""},
#line 166 "attributes.gperf"
      {"rows", VHTMLATTR_ROWS},
      {""}, {""}, {""},
#line 73 "attributes.gperf"
      {"low", VHTMLATTR_LOW},
#line 194 "attributes.gperf"
      {"html VHTMLATTR_HTML"},
#line 40 "attributes.gperf"
      {"contenteditable", VHTMLATTR_CONTENTEDITABLE},
#line 122 "attributes.gperf"
      {"onmouseover", VHTMLATTR_ONMOUSEOVER},
      {""},
#line 47 "attributes.gperf"
      {"dir", VHTMLATTR_DIR},
#line 28 "attributes.gperf"
      {"autofocus", VHTMLATTR_AUTOFOCUS},
      {""},
#line 160 "attributes.gperf"
      {"poster", VHTMLATTR_POSTER},
#line 108 "attributes.gperf"
      {"onfocus", VHTMLATTR_ONFOCUS},
#line 163 "attributes.gperf"
      {"rel", VHTMLATTR_REL},
      {""}, {""},
#line 188 "attributes.gperf"
      {"usemap", VHTMLATTR_USEMAP},
#line 107 "attributes.gperf"
      {"onerror", VHTMLATTR_ONERROR},
#line 139 "attributes.gperf"
      {"onsearch", VHTMLATTR_ONSEARCH},
      {""},
#line 128 "attributes.gperf"
      {"onpageshow", VHTMLATTR_ONPAGESHOW},
      {""},
#line 155 "attributes.gperf"
      {"pattern", VHTMLATTR_PATTERN},
#line 85 "attributes.gperf"
      {"onbeforeprint", VHTMLATTR_ONBEFOREPRINT},
#line 86 "attributes.gperf"
      {"onbeforeunload", VHTMLATTR_ONBEFOREUNLOAD},
      {""}, {""},
#line 57 "attributes.gperf"
      {"headers", VHTMLATTR_HEADERS},
      {""}, {""}, {""},
#line 156 "attributes.gperf"
      {"placeholder", VHTMLATTR_PLACEHOLDER},
      {""}, {""}, {""}, {""},
#line 119 "attributes.gperf"
      {"onmousedown", VHTMLATTR_ONMOUSEDOWN},
      {""}, {""}, {""},
#line 69 "attributes.gperf"
      {"label", VHTMLATTR_LABEL},
      {""},
#line 91 "attributes.gperf"
      {"onclick", VHTMLATTR_ONCLICK},
      {""},
#line 22 "attributes.gperf"
      {"accesskey", VHTMLATTR_ACCESSKEY},
      {""},
#line 131 "attributes.gperf"
      {"onplay", VHTMLATTR_ONPLAY},
#line 45 "attributes.gperf"
      {"default", VHTMLATTR_DEFAULT},
      {""},
#line 55 "attributes.gperf"
      {"form", VHTMLATTR_FORM},
      {""}, {""},
#line 33 "attributes.gperf"
      {"checked", VHTMLATTR_CHECKED},
#line 29 "attributes.gperf"
      {"autoplay", VHTMLATTR_AUTOPLAY},
#line 70 "attributes.gperf"
      {"lang", VHTMLATTR_LANG},
#line 96 "attributes.gperf"
      {"ondblclick", VHTMLATTR_ONDBLCLICK},
#line 93 "attributes.gperf"
      {"oncopy", VHTMLATTR_ONCOPY},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 75 "attributes.gperf"
      {"maxlength", VHTMLATTR_MAXLENGTH},
#line 26 "attributes.gperf"
      {"async", VHTMLATTR_ASYNC},
      {""}, {""},
#line 78 "attributes.gperf"
      {"min", VHTMLATTR_MIN},
      {""}, {""},
#line 87 "attributes.gperf"
      {"onblur", VHTMLATTR_ONBLUR},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 162 "attributes.gperf"
      {"readonly", VHTMLATTR_READONLY},
      {""}, {""}, {""},
#line 30 "attributes.gperf"
      {"bgcolor", VHTMLATTR_BGCOLOR},
#line 92 "attributes.gperf"
      {"oncontextmenu", VHTMLATTR_ONCONTEXTMENU},
      {""},
#line 175 "attributes.gperf"
      {"spellcheck", VHTMLATTR_SPELLCHECK},
      {""}, {""},
#line 158 "attributes.gperf"
      {"popovertarget", VHTMLATTR_POPOVERTARGET},
      {""},
#line 63 "attributes.gperf"
      {"http-equiv", VHTMLATTR_HTTPEQUIV},
#line 31 "attributes.gperf"
      {"border", VHTMLATTR_BORDER},
      {""},
#line 62 "attributes.gperf"
      {"hreflang", VHTMLATTR_HREFLANG},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 183 "attributes.gperf"
      {"tabindex", VHTMLATTR_TABINDEX},
      {""}, {""},
#line 59 "attributes.gperf"
      {"hidden", VHTMLATTR_HIDDEN},
      {""}, {""},
#line 61 "attributes.gperf"
      {"href", VHTMLATTR_HREF},
#line 24 "attributes.gperf"
      {"align", VHTMLATTR_ALIGN},
      {""}, {""},
#line 41 "attributes.gperf"
      {"controls", VHTMLATTR_CONTROLS},
      {""},
#line 36 "attributes.gperf"
      {"color", VHTMLATTR_COLOR},
      {""},
#line 38 "attributes.gperf"
      {"colspan", VHTMLATTR_COLSPAN},
      {""},
#line 88 "attributes.gperf"
      {"oncanplay", VHTMLATTR_ONCANPLAY},
      {""}, {""},
#line 168 "attributes.gperf"
      {"sandbox", VHTMLATTR_SANDBOX},
      {""}, {""}, {""},
#line 58 "attributes.gperf"
      {"height", VHTMLATTR_HEIGHT},
      {""}, {""},
#line 68 "attributes.gperf"
      {"kind", VHTMLATTR_KIND},
#line 82 "attributes.gperf"
      {"novalidate", VHTMLATTR_NOVALIDATE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 165 "attributes.gperf"
      {"reversed", VHTMLATTR_REVERSED},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 89 "attributes.gperf"
      {"oncanplaythrough", VHTMLATTR_ONCANPLAYTHROUGH},
      {""},
#line 54 "attributes.gperf"
      {"for", VHTMLATTR_FOR},
      {""},
#line 113 "attributes.gperf"
      {"onkeypress", VHTMLATTR_ONKEYPRESS},
      {""}, {""}, {""}, {""},
#line 46 "attributes.gperf"
      {"defer", VHTMLATTR_DEFER},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 193 "attributes.gperf"
      {"xmlns:xlink", VHTMLATTR_XMLNS_XLINK},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 60 "attributes.gperf"
      {"high", VHTMLATTR_HIGH},
      {""}, {""},
#line 157 "attributes.gperf"
      {"popover", VHTMLATTR_POPOVER},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 167 "attributes.gperf"
      {"rowspan", VHTMLATTR_ROWSPAN},
      {""}, {""},
#line 190 "attributes.gperf"
      {"width", VHTMLATTR_WIDTH},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 114 "attributes.gperf"
      {"onkeyup", VHTMLATTR_ONKEYUP},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 159 "attributes.gperf"
      {"popovertargetaction", VHTMLATTR_POPOVERTARGETACTION},
#line 56 "attributes.gperf"
      {"formaction", VHTMLATTR_FORMACTION},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 112 "attributes.gperf"
      {"onkeydown", VHTMLATTR_ONKEYDOWN}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = html_attribute_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = html_attribute_pool[key].attr_name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &html_attribute_pool[key];
        }
    }
  return (struct VHTMLAttribute_st *) 0;
}
#line 195 "attributes.gperf"

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
	[VHTMLATTR_XMLNS] = "xmlns", [VHTMLATTR_XMLNS_XLINK] = "xmlns:xlink", [VHTMLATTR_HTML] = "html"
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

VHTMLAttribute* set_attr(VHTMLArena *arena, VHTMLNode *node, VHTMLAttribute_t attribute, char *value){
	if(!node) return NULL;
	VHTMLAttribute* curr = node->attributes;
	if(curr){
		while(curr->type != attribute && curr->next) curr = curr->next;
		if(curr->type == attribute){
			curr->value = value;
			return curr;
		}
	}

	VHTMLAttribute* new_attr = arena_alloc(arena, sizeof(VHTMLAttribute), alignof(VHTMLAttribute));
	if(!new_attr) return NULL;
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
            parent->last_child = next_child;
        } else {
            parent->last_child->forward_sibling = next_child;
            parent->last_child = next_child;
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


void free_document(VHTMLDocument *doc){
	if(doc){
		if(doc->raw_buf) VHTML_FREE(doc->raw_buf);
		VHTML_FREE(doc);
	}
}



#define P_ATTR(v, q, fmt, ...) do { \
    char _q[] = {(q), '\0'}; \
    if (format_style == VHTMLPRINT_DEBUG) fprintf(f, "\t" fmt ": %s\n", __VA_ARGS__, (v) ? (v) : ""); \
    else fprintf(f, " " fmt "%s%s%s%s", __VA_ARGS__, (v)?"=":"", (q) ? _q : ((v) ? "\"" : ""), (v)?(v):"", (q) ? _q : ((v) ? "\"" : "")); \
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

// -------- Util Functions -----------

VHTMLNode *append_child(VHTMLNode *parent, VHTMLNode *child){
	if(!parent || !child) return NULL;
	if(!parent->children){
		parent->children = child; // no children case
	} else if(parent->last_child) {
		parent->last_child->forward_sibling = child; // append to end of ll
	}
	parent->last_child = child; // for fast appending
	return child;
}

VHTMLNode *set_inner_html(VHTMLArena *arena, VHTMLNode *parent, char *str){
	if(!parent || !str) return NULL;
	size_t len = strlen(str) + 1;
	void *alloc = arena_alloc(arena, len, alignof(char));
	if(!alloc) return NULL;
	str = memcpy(alloc, str, len);
	parent->children = NULL;
	return parse_node(arena, str, parent);
}

VHTMLNode *set_inner_text(VHTMLArena *arena, VHTMLNode *parent, char *str){
	if(!parent || !arena || !str) return NULL;
	size_t len = strlen(str) + 1;
	void *alloc = arena_alloc(arena, len, alignof(char));
	if(!alloc) return NULL;
	str = memcpy(alloc, str, len);
	VHTMLNode *textNode = create_element_t(_VHTMLTAG_TEXT, arena);
	textNode->_text = str;
	parent->children = textNode; // children are lost to time
	return textNode;
}

/* 
 * Nondestructively parses a string into a DOM tree, returning a pointer to the heap allocated document struct.
 * It is important to note that if `arena` is NULL, the document will allocate an arena itsself of size FIXME bytes
 * which would need to be freed by caller to avoid leaks.
 *
 * If no string is provided, the document is created with the standard:
 * ```html
 * <!DOCTYPE html>
 * <html>
 *	<head></head>
 *	<body></body>
 * </html>
 * ```
 * */
VHTMLDocument* create_document(VHTMLArena *arena, char *str){
	int owned_arena = !arena;
	if(owned_arena) arena = arena_init(4098);

	VHTMLDocument *doc = VHTML_CALLOC(1, sizeof(VHTMLDocument));
	if(!doc) goto cleanup;

	doc->allocation = arena;
	doc->root = create_element_t(_VHTMLTAG_ROOT, arena);
	doc->owns_allocations = owned_arena;

	int errs = 0;
	
	if(str){
		doc->raw_buf = strdup(str);
		parse_node(arena, doc->raw_buf, doc->root);
	} else { // base template
		errs |= !append_child(doc->root, create_element_t(VHTMLTAG_DOCTYPE, arena));
		errs |= !set_attr(arena, doc->root->children, VHTMLATTR_HTML, 0);
		VHTMLNode *root = create_element_t(VHTMLTAG_HTML, arena);
		errs |= !append_child(doc->root, root);
		errs |= !append_child(root, create_element_t(VHTMLTAG_HEAD, arena));
		errs |= !append_child(root, create_element_t(VHTMLTAG_BODY, arena));
		doc->body = root->last_child; // last child
		if(errs) goto cleanup;
	}

	return doc;

cleanup:
	if(doc){
		if(doc->raw_buf) VHTML_FREE(doc->raw_buf);
		if(doc->owns_allocations) arena_free(doc->allocation);
		VHTML_FREE(doc);
	}
	return NULL;
}

// Returns heap-allocated buffer of all text in children
char *get_text_content(VHTMLNode *node, VHTMLArena *arena) {
    if (!node) return NULL;
    if (node->type == _VHTMLTAG_TEXT) return node->_text;

    size_t total = 0;
    for (VHTMLNode *ch = node->children; ch; ch = ch->forward_sibling) {
        char *t = get_text_content(ch, arena);
        if (t) total += strlen(t);
    }

    char *buf = arena_alloc(arena, total + 1, 1);
    buf[0] = '\0';
    for (VHTMLNode *ch = node->children; ch; ch = ch->forward_sibling) {
        char *t = get_text_content(ch, arena);
        if (t) strcat(buf, t);
    }
    return buf;
}
#endif /* VHTML_IMPLEMENTATION */
