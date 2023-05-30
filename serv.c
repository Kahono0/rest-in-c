#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // for write()

#define PORT 4000


//define request struct
typedef struct request{
    char* method;
    char* path;
    char* version;
    char* headers;
    char* body;
} Request;

//define response struct
typedef struct response{
    char* version;
    char* status;
    char* headers;
    char* body;
} response;

//parse string request into request struct
Request parseRequest(char* request){
    Request r;
    char* token = strtok(request, " ");
    r.method = token;
    token = strtok(NULL, " ");
    r.path = token;
    token = strtok(NULL, " ");
    r.version = token;
    token = strtok(NULL, " ");
    r.headers = token;
    token = strtok(NULL, " ");
    r.body = token;
    return r;
}


char* simple_tokenizer(char* s){
    char* token = strtok(s, " ");
    return token;
}


void handle(int new_client){
    char request[1000];
    read(new_client, request, 1000);
    printf("got request: %s\n", request);
    Request r = parseRequest(request);

    printf("headers: %s\n", r.headers);


 // construct a response
    char *status_line = "HTTP/1.1 200 OK\r\n";
    char *headers = "Content-Type: text/html\r\n";
    char *body = "<html><body><h1>Hello, Chad!</h1></body></html>\r\n";
    int content_length = strlen(body);
    char length_str[50];
    sprintf(length_str, "Content-Length: %d\r\n", content_length);
    char response[1000];
    sprintf(response, "%s%s%s\r\n%s", status_line, headers, length_str, body);
    // send the response to the client
    write(new_client, response, strlen(response));

    printf("sent response:\n%s\n", response);
}

int main() {
  printf("Running Server on localhost:%d \n", PORT);

  //printf("%s", simple_tokenizer("GET / HTTP/1.1"));

  // create a socket
  int the_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (the_socket < 0) {
    printf("could not create socket 😭");
    return 1;
  }

  // bind the_socket to port 4000
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT); // port 4000
  if (bind(the_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("could not bind socket 😭");
    return 1;
  }

  // listen for connections
  listen(the_socket, 10);

  for (;;) {
    // accept connections
    int new_client = accept(the_socket, NULL, NULL);
    if (new_client < 0) {
      printf("could not accept connection 😭");
      return 1;
    }

    //read request

    // connected to a client!
    printf("connected to client 🥳\n");
    handle(new_client);
}

  return 0;
}
