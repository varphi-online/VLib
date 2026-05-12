#ifndef VHTTP_H
#define VHTTP_H

#include <netdb.h>
#include <stdbool.h>
#include <sys/socket.h>

enum vhttp_method {
  VHTTPMETHOD_GET,
  VHTTPMETHOD_HEAD,
  VHTTPMETHOD_POST,
  VHTTPMETHOD_OPTIONS,
  VHTTPMETHOD_TRACE,
  VHTTPMETHOD_PUT,
  VHTTPMETHOD_DELETE,
  VHTTPMETHOD_PATCH,
  VHTTPMETHOD_CONNECT,
  VHTTPMETHOD_LINK,
  VHTTPMETHOD_UNLINK
};

extern const char *const vhttp_method_str[];

enum vhttp_version {
  VHTTP_0_9,
  VHTTP_1_0,
  VHTTP_1_1,
  VHTTP_2_0,
  VHTTP_3_0,
};

extern const char *const vhttp_version_str[];

typedef struct vhttp_request_line {
  enum vhttp_method method;
  char *request_uri;
  enum vhttp_version vhttp_version;
} vhttp_request_line;

/* VHTTP */
typedef struct vhttp_header {
  char *key;
  char *value;
  struct vhttp_header *next;
} vhttp_header;

typedef struct vhttp_request_headers {
  struct vhttp_header *head;
  struct vhttp_header *tail;
  size_t size;
  char *_header_buf;
  int _bufsize;
} vhttp_request_headers;

typedef struct vhttp_request {
  vhttp_request_line *request_line;
  vhttp_request_headers *headers;
  char *body;
  int _client_fd;
} vhttp_request;

typedef struct vhttp_response {
  int status;
  char *headers;
  char *body;
} vhttp_response;

typedef struct vhttp_server {
  int _socket, _current_accept;
  struct sockaddr_storage _connecting_addr;
  socklen_t _connecting_addr_size;
  struct addrinfo _hints, *res;
  void (*entrypoint)(vhttp_request *, vhttp_response *, void **);
  int concurrent_connections;
  void **context;
} vhttp_server;

enum vhttp_status { VHTTP_OK = 200, VHTTP_NOT_FOUND = 404 };

#define CONTENT_TYPE_TEXT "text/plain"
#define CONTENT_TYPE_JSON "application/json"

vhttp_server *vhttp_server_init(char *port,
                             void (*entrypoint)(vhttp_request *, vhttp_response *, void **),
                             void **context);

// Expects a heap-allocated vhttp_request
// Destroys input string
int vhttp_parse_request(char *request_str, vhttp_request *request);

void vhttp_server_listen(struct vhttp_server *server);

int vhttp_respond(struct vhttp_request *request, struct vhttp_response *response);

void vhttp_set_response_status(struct vhttp_response *response, int status);

void vhttp_set_response_header(struct vhttp_response *response, char *key,
                              char *value);
char *vhttp_headers_to_string(struct vhttp_request_headers *headers,
                             bool pretty_print);

#endif
