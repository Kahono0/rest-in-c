#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> // for inet_ntoa()
#include <unistd.h> // for write()
                    //
//prototypes
void handleRequest(char r, int new_client, char (*handler)(char));
char handler(char r);

void handleRequest(char r, int new_client, char (*handler)(char)){
    //w is the response and r is the request string
    char response = handler(r);
    write(new_client, &response, strlen(&response));
}

char handler(char r){
    //w is the response and r is the request string
    char *status_line = "HTTP/1.1 200 OK\r\n";
    char *headers = "Content-Type: text/html\r\n";
    char *body = "<html><body><h1>Hello, Chad!</h1></body></html>\r\n";
    int content_length = strlen(body);
    char length_str[50];
    sprintf(length_str, "Content-Length: %d\r\n", content_length);
    char response[1000];
    sprintf(response, "%s%s%s\r\n%s", status_line, headers, length_str, body);
    return *response;
}

int main() {
  printf("Running Server on localhost:4000 \n");

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
  addr.sin_port = htons(4012); // port 4000
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

    //get client ip address
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
    printf("got connection from %s\n", str);

    //get request from new_client
    char request[1000];
    read(new_client, request, 1000);
    printf("got request: %s\n", request);

    // connected to a client!
    printf("connected to client 🥳\n");


    // handle the handleRequest
    handleRequest(*request, new_client, handler);
  }

  return 0;
}
