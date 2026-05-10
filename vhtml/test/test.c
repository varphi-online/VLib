#include <stdio.h>
#include <string.h>
//#define VHTML_IMPLEMENTATION
//#include "vhtml.h"
#include "../vhtml_h.h"

static char *test_str = 
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


int main(){
	VHTMLArena *arena = arena_init(2048);		
	char *dup = strdup(test_str);
	puts("Expected:");
	puts(dup);
	VHTMLNode *node = parse_node(arena, dup, 0);
	puts("\nRecovered:");
	printNode(node, VHTMLPRINT_DEBUG);
	printNode(node, VHTMLPRINT_PRETTY);
	printNode(node, VHTMLPRINT_COMPACT);
	free(dup);
	arena_free(arena);
}





