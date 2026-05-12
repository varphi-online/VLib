# Vhttp

Example:
```c
#include <stdio.h>
#define VHTTP_IMPLEMENTATION
#include "../vhttp.h"

void req_handle(vhttp_request *req, vhttp_response *resp, void **ctx){
	printf("%s\r\n%s", vhttp_headers_to_string(req->headers, 1), req->body);
	resp->body = "hello!\n";
	resp->status = VHTTP_OK;
	vhttp_respond(req, resp);
}

int main(){
    vhttp_server *server = vhttp_server_init("8080", req_handle, NULL);
	vhttp_server_listen(server);
    return 0;
}
```
