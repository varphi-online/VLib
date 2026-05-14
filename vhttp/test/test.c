#include <stdio.h>
#define VHTTP_IMPLEMENTATION
#include "../vhttp.h"

#define VHTML_IMPLEMENTATION
#include "../../vhtml/vhtml.h"

void req_handle(vhttp_request *req, vhttp_response *resp, void **ctx){
	VHTMLNode *ul = ctx[0];
	VHTMLDocument *doc = ctx[1];
	printf("%s\r\n%s", vhttp_headers_to_string(req->headers, 1), req->body);

	VHTMLNode *li = create_element_t(VHTMLTAG_LI, doc->allocation);
	set_inner_text(doc->allocation, li, req->request_line->request_uri);
	append_child(ul, li);


	resp->body = serializeDocument(doc, VHTMLPRINT_COMPACT);
	resp->status = VHTTP_OK;
	vhttp_respond(req, resp);

	free(resp->body);
}

int main(){
	VHTMLDocument *doc = create_document(0, 0);
	set_inner_html(doc->allocation, doc->body, "<div><ul></ul></div>");
	set_attr(doc->allocation, doc->body, VHTMLATTR_STYLE, "background-color: black; color: white;");
	set_attr(doc->allocation, doc->body->children, VHTMLATTR_STYLE, "width: 100%; text-align: center;");

	VHTMLNode *ul = doc->body->children->children;
	printNode(ul, VHTMLPRINT_DEBUG);

    vhttp_server *server = vhttp_server_init("8080", req_handle, (void *[]){ul, doc});
	vhttp_server_listen(server);
    return 0;
}
