#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define VHTML_IMPLEMENTATION
#include "../vhtml.h"
#include <stdio.h>

static char *tag_str = "<marquee  data-🚀-emoji='quoted \"value\" with &amp; entities'"
"  contenteditable=\"\""
"  xml:lang = \"en-us\""
"  hidden"
"  style = 'background: url(\"data:image/svg+xml;utf8,<svg/>\"); content: \"}\"; '"
"  onclick = \"alert(`tricky`backtick`)\""
"  data-json = '{\"key\": \"value with </marquee> fake closer\"}'"
"  class=foo&#x20;bar"
"  data-unicode-name=\"zero-width joiner in name\""
"  itemscope itemtype=https://schema.org/thing><img/> boxed <br><br> <span>hello</span></marquee  >\n";

static char *long_str = 
"<div class=\"app\" data-window_title=\"General Info\" data-icon_name=\"Info\" data-icon=\"Help book.ico\" data-movable=\"\" style=\"right: 12vw; top: 47vh; display: none; position: absolute; z-index: 948;\" id=\"General_Info269-window\"><div class=\"window win95\">\n"
"      <div class=\"windowTitlebar\" style=\"cursor: pointer;\">\n"
"        <div class=\"windowTitleArea\">\n"
"          <p class=\"windowTitle\">General Info</p>\n"
"        </div>\n"
"        <div class=\"windowTitlebarInteractableArea\">\n"
"          <button class=\"windowExtern win95\" style=\"display: none;\">🡕</button>\n"
"          <button class=\"windowMinimize win95\">⎽</button>\n"
"          <button class=\"windowMaximize win95\" style=\"display: none;\">⧠</button>\n"
"          <button class=\"windowClose win95\" style=\"display: none;\">⨯</button>\n"
"        </div>\n"
"      </div>\n"
"      <div class=\"windowSurface\">\n"
"    <div style=\"\n"
"          background-color: #f1f0f1;\n"
"          color: black;\n"
"          width: 30vh;\n"
"          margin: 2px;\n"
"        \">\n"
"      <p style=\"margin: 2px; font-size: 0.85em; text-align: center\">\n"
"        Varphi is the collective works of musician, programmer and jack of\n"
"        trades &lt;REDACTED&gt;<br><br>\n"
"        This site is a compilation of all works that can be faithfully brought\n"
"        to the world-wide-web and is very much alive! Constantly being\n"
"        updated, with changes plainly visible on the accompanying Github Repo.\n"
"        Please feel free to leave a message in the guestbook or look around at other\n"
"        projects!\n"
"        <br> <br>\n"
"        Anything under the non-subdomain paths of this site are written exclusively\n"
"        in HTML, CSS and JS (Typescript) without any frameworks or other tools than\n"
"        a TS compiler.\n"
"      </p>\n"
"    </div>\n"
"  </div>\n"
"    </div></div>";

static uint64_t diff_ns(struct timespec start, struct timespec end) {
    int64_t sec = (int64_t)end.tv_sec - (int64_t)start.tv_sec;
    int64_t nsec = (int64_t)end.tv_nsec - (int64_t)start.tv_nsec;
    return (uint64_t)(sec * 1000000000LL + nsec);
}

int main(int argc, char **argv){
	int count = 1500000;
	FILE *data = 0;
	if (argc == 2){
		count = atoi(argv[1]);
		if(!count){
			data = fopen(argv[1], "r");
			count = 1500000;
		}
	} else if (argc == 3){
		data = fopen(argv[1], "r");
		count = atoi(argv[2]);
	}

	char *str = long_str;

	if(data){
		fseek(data, 0, SEEK_END);
		size_t size = ftell(data);
		rewind(data);
		str = malloc(size+1);
		int _ = fread(str, 1, size, data);
		str[size] = '\0';
	}

	//puts(str);

	VHTMLDocument *test = 0; // so we dont have to check if null
													   // each loop

	uintptr_t x = 0;
	struct timespec start, end;
	VHTMLArena *arena = arena_init(4098);
	char **strs = malloc(sizeof(char *) * count);
	for(int i = 0; i<count; i++){
		strs[i] = strdup(str);
	}

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int i = 0; i<count; i++){
		if(test) free_document(test);
		arena_reset(arena);
		test = create_document(arena, strs[i]);
		x ^= (uintptr_t)test;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);


	char *serialized;// = serializeDocument(test, VHTMLPRINT_DEBUG);
/*
	puts("Debug:\n");
	puts(serialized);
	free(serialized);
*/
	puts("Serialized\n");
	serialized = serializeDocument(test, VHTMLPRINT_PRETTY);
	puts(serialized);
	free(serialized);
	free_document(test);
	arena_free(arena);
	for(int i = 0; i<count; i++){
		free(strs[i]);
	}
	free(strs);
	uint64_t elapsed = diff_ns(start, end);
    printf("\nParsed %d inputs in: %.2fms (~%.4fμs / ~%lluns avg/run)\nMib/s: %.2f\n",
			count,
			(double)(elapsed/ 1000000.0),
			(double)(elapsed/((double)count*1000)),
			(unsigned long long)(elapsed/(double)count),
			((strlen(str) * count) / (elapsed / 1e9)) / (1024.0 * 1024.0));
	volatile uintptr_t sink = x;
	if(data) free(str);
	if(data) fclose(data);
}
