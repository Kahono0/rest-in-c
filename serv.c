#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // for write()
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
    Status *status;
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

char* subString(char* string, int start, int end){
    char* sub = malloc(end - start + 1);
    int i = 0;
    for(int j = start; j < end; j++){
        sub[i] = string[j];
        i++;
    }
    sub[i] = '\0';
    return sub;
}

void parseHeader(char* string, Header *header) {
    header->key = malloc(strlen(string) + 1);
    header->value = malloc(strlen(string) + 1);

    sscanf(string, "%s %s", header->key, header->value);

    header->key = subString(header->key, 0, strlen(header->key) - 1);
}


void parseStatus(char* string, Status *status) {
    status->method = malloc(strlen(string) + 1);
    status->path = malloc(strlen(string) + 1);
    status->version = malloc(strlen(string) + 1);

    sscanf(string, "%s %s %s", status->method, status->path, status->version);

}

//parse string request into request struct
void parseRequest(char* request, Request *req){
    char *copy = strdup(request);

    char *line = strtok(copy, "\n");
    Status *status = malloc(sizeof(Status));
    parseStatus(line, status);
    req->status = status;
    int i = 0;
    while(line != NULL){
        line = strtok(NULL, "\r\n");
        if(line != NULL){
            Header *header = malloc(sizeof(Header));
            parseHeader(line, header);
            req->headers[i] = *header;

            if(strcmp(header->key, "Content-Length") == 0){

                line = strtok(NULL, "");

                req->body = malloc(strlen(line) + 1);
                strncpy(req->body, line, strlen(line) + 1);
                req->body[strlen(line)] = '\0';
            }


        }
        //printf("line: %s\n", line);
        if(line != NULL){
            i++;

        }
    }

    free(copy);
}

void handle(int new_client){
    char request[1000];
    read(new_client, request, 1000);
    Request *req = malloc(sizeof(Request));
    parseRequest(request, req);

    printf("method: %s\n", req->status->method);

    printf("Headers: \n");
    for(int i = 0; i < MAX_HEADER_LENGTH; i++){
        if(req->headers[i].key != NULL){
            printf("%s: %s\n", req->headers[i].key, req->headers[i].value);
        }
    }

    printf("body: %s\n", req->body);


    //construct a response
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


  int i = 0;
  clock_t start;
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
    if(i == 0){
        start = clock();
    }
    i++;
    printf("i: %d\n", i);
    if(i==1000){
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("time spent: %f\n", time_spent);
        break;
    }
  }

  // close the the_socket
  close(the_socket);

  return 0;
}
