#include <stdio.h>
#define VHTTP_IMPLEMENTATION
#include "../vhttp.h"

#define VHTML_IMPLEMENTATION
#include "../../vhtml/vhtml.h"

void req_handle(vhttp_request *req, vhttp_response *resp, void **ctx){
	printf("%s\r\n%s", vhttp_headers_to_string(req->headers, 1), req->body);
	VHTMLDocument *doc = document_from_string(NULL, "");
	doc->root->children = create_element("p", doc->allocation);
	doc->root->children->children = create_element_t(_VHTMLTAG_TEXT, doc->allocation);
	doc->root->children->children->_text = "boxed";
	resp->body = serializeDocument(doc, VHTMLPRINT_PRETTY);
	resp->status = VHTTP_OK;
	vhttp_respond(req, resp);
	free(resp->body);
	free_document(doc);
}

int main(){
    vhttp_server *server = vhttp_server_init("8080", req_handle, NULL);
	vhttp_server_listen(server);
    return 0;
}
