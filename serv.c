#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // for write()
#include <stdlib.h>
#include <string.h>


#define MAX_HEADER_LENGTH 1000
//define request struct
typedef struct header {
    char* key;
    char* value;
} Header;

typedef struct status {
    char* method;
    char* path;
    char* version;
} Status;

typedef struct request{
    char* method;
    char* path;
    char* version;
    Header headers[MAX_HEADER_LENGTH];
    char* body;
} Request;

//define response struct
typedef struct response{
    char* version;
    char* status;
    Header* headers;
    char* body;
} response;

Header parseHeader(const char* string) {
  Header header;

  // Find the first occurrence of the colon ':' in the string.
  size_t colon_index = strcspn(string, ":");

  // Copy the substring up to the colon into the header's key.
  header.key = malloc(colon_index + 1);
  strncpy(header.key, string, colon_index);
//   header.key[colon_index] = '\0';

  // Copy the substring after the colon into the header's value.
  header.value = malloc(strlen(string) - colon_index + 1);
  strncpy(header.value, string + colon_index + 1, strlen(string) - colon_index);
//   header.value[strlen(string) - colon_index] = '\0';

  return header;
}

Status parseStatus(char* string) {
  Status status;

  // Split GET / HTTP/1.1
    char* token = strtok(string, " ");
    status.method = token;
    token = strtok(NULL, " ");
    status.path = token;
    token = strtok(NULL, " ");
    status.version = token;

    return status;
}

//parse string request into request struct
Request parseRequest(char* request){
    Request r;
    int i = 0;
    char lines[100][1000];
    char token[1000];
    int lineCount = 0;
    int headerCount = 0;
    while(request[i] != '\0'){
        if(request[i] == '\n'){
            strcpy(lines[lineCount], token);
            lineCount++;
            if(lineCount != 1){
                Header h = parseHeader(token);
                r.headers[lineCount] = h;
            }else{
                printf("first line: %s\n", token);
                Status s = parseStatus(token);
                r.method = s.method;
                r.path = s.path;
                r.version = s.version;

            }
            strcpy(token, "");

        }else{
            strncat(token, &request[i], 1);
        }
        i++;
    }

    return r;
}

void handle(int new_client){
    char request[1000];
    read(new_client, request, 1000);
    // printf("got request: %s\n", request);z
    Request r = parseRequest(request);

    printf("Method, version and path\n");
    printf("%s\n", r.method);
    printf("%s\n", r.version);
    printf("%s\n", r.path);



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

    // close the connections
    close(new_client);

}

int main(char argc, char *argv[]) {
    //to int
    int PORT = atoi(argv[1]);
  printf("Running Server on http://localhost:%d \n", PORT);

  //printf("%s", simple_tokenizer("GET / HTTP/1.1"));

  // create a socket
  int the_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (the_socket < 0) {
    printf("socket: %d\n", the_socket);
    return 1;
  }

  // bind the_socket to port 4000
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT); // port 4000
  if (bind(the_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("could not bind socket \n");
    return 1;
  }

  // listen for connections
  listen(the_socket, 10);

  for (;;) {
    // accept connections
    int new_client = accept(the_socket, NULL, NULL);
    if (new_client < 0) {
      printf("could not accept connection\n");
      return 1;
    }

    //read request

    // connected to a client!
    printf("connected to client \n");
    handle(new_client);
  }

  // close the the_socket
  close(the_socket);

  return 0;
}
