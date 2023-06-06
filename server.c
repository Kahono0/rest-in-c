// /*
// #include<stdio.h>
// #include <string.h>

// typedef struct request{
//     char* method;
//     char* path;
//     char* version;
//     char* headers;
//     char* body;
// } Request;

// int main() {                              //
//    FILE *fp = fopen("req.txt", "r");
//    if (fp == NULL) {
//       printf("Error opening file\n");
//       return 1;
//    }

//    //read lines

//    char line[1000];

//    fgets(line, sizeof(line), fp);
//    Request r;
//    char* token = strtok(line, " ");
//    printf("method: %s\n", r.method);
//    //headers
//    char* headers[50];

//    int i = 0;
//    while (fgets(line, sizeof(line), fp)) {
//       //printf("%s", line);
//       headers[i] = line;
//       i++;
//    }
//    printf("%s\n", r.method);
//    if(strcmp(r.method, "GET") != 0){
//        //remove body from headers
//        r.body = headers[i-1];
//    }else{
//        printf("no body\n");
//    }

//    return 0;
// }*/

// #include <stdio.h>
// #include <string.h>

// #define MAX_PAIRS 2
// #define MAX_KEY_LENGTH 100
// #define MAX_VALUE_LENGTH 100

// int main() {
//     char input[] = "hello:world\nmy:name";

//     char pairs[MAX_PAIRS][2][MAX_VALUE_LENGTH];

//     // Tokenize the input string
//     char* token;
//     int row = 0;
//     int col = 0;

//     token = strtok(input, "\n:");
//     while (token != NULL) {
//         strcpy(pairs[row][col], token);

//         col++;
//         if (col == 2) {
//             col = 0;
//             row++;
//         }

//         token = strtok(NULL, "\n:");
//     }

//     // Print the two-dimensional array
//     for (int i = 0; i < MAX_PAIRS; i++) {
//         printf("%s, %s\n", pairs[i][0], pairs[i][1]);
//     }

//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef struct {
//     char* key;
//     char* value;
// } Header;

// typedef struct {
//     char* method;
//     char* path;
//     char* version;
//     Header* headers;
//     int num_headers;
// } Request;

// Request parseHttpRequest(const char* requestString) {
//     Request request;

//     char* mutableRequestString = strdup(requestString);
//     char* line = strtok(mutableRequestString, "\r\n");

//     // Parse the first line (method, path, version)
//     char* token = strtok(line, " ");
//     request.method = strdup(token);

//     token = strtok(NULL, " ");
//     request.path = strdup(token);

//     token = strtok(NULL, " ");
//     request.version = strdup(token);

//     // Parse the headers
//     request.headers = NULL;
//     request.num_headers = 0;

//     while ((line = strtok(NULL, "\r\n")) != NULL) {
//         Header header;

//         char* colon = strchr(line, ':');
//         if (colon != NULL) {
//             *colon = '\0';
//             header.key = strdup(line);
//             header.value = strdup(colon + 2); // Skip the space after the colon

//             request.headers = realloc(request.headers, (request.num_headers + 1) * sizeof(Header));
//             request.headers[request.num_headers++] = header;
//         }
//     }

//     free(mutableRequestString);

//     return request;
// }

// int main() {
//     const char* requestString = "GET / HTTP/1.1\r\n"
//                                 "Host: localhost:4000\r\n"
//                                 "Connection: keep-alive\r\n"
//                                 "Cache-Control: max-age=0\r\n"
//                                 "sec-ch-ua: \"Google Chrome\";v=\"113\", \"Chromium\";v=\"113\", \"Not-A.Brand\";v=\"24\"\r\n"
//                                 "sec-ch-ua-mobile: ?0\r\n"
//                                 "sec-ch-ua-platform: \"Linux\"\r\n"
//                                 "Upgrade-Insecure-Requests: 1\r\n"
//                                 "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36\r\n"
//                                 "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
//                                 "Sec-Fetch-Site: none\r\n"
//                                 "Sec-Fetch-Mode: navigate\r\n"
//                                 "Sec-Fetch-User: ?1\r\n"
//                                 "Sec-Fetch-Dest: document\r\n"
//                                 "Accept-Encoding: gzip, deflate, br\r\n"
//                                 "Accept-Language: en-US,en-KE;q=0.9,en;q=0.8\r\n"
//                                 "Cookie: _smart_farm_key=SFMyNTY.g3QAAAACbQAAAAtfY3NyZl90b2tlbm0AAAAYaHVZckhESTBfYzNpckU5WG5uOS1zeC02bQAAAAd1c2VyX2lkbQAAACQ4ZWEwOTljYy0xOTdjLTQ2ZDAtODE4Zi0yZWNmMTgyOGM0MmQ.KvFvmeyrOij5sUB3LBKzjkoIvlFuDCVovP8t0hWzoO0\r\n";

//     Request request = parseHttpRequest(requestString);

//     printf("Method: %s\n", request.method);
//     printf("Path: %s\n", request.path);
//     printf("Version: %s\n", request.version);

//     for (int i = 0; i < request.num_headers; i++) {
//         printf("Header: %s - %s\n", request.headers[i].key, request.headers[i].value);
//     }

//     free(request.method);
//     free(request.path);
//     free(request.version);
//     for (int i = 0; i < request.num_headers; i++) {
//         free(request.headers[i].key);
//         free(request.headers[i].value);
//     }
//     free(request.headers);

//     return 0;
// }
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct header {
    char* key;
    char* value;
} Header;

Header split_header(const char* string) {
  Header header;

  // Find the first occurrence of the colon ':' in the string.
  size_t colon_index = strcspn(string, ":");

  // Copy the substring up to the colon into the header's key.
  header.key = malloc(colon_index + 1);
  strncpy(header.key, string, colon_index);
  header.key[colon_index] = '\0';

  // Copy the substring after the colon into the header's value.
  header.value = malloc(strlen(string) - colon_index + 1);
  strncpy(header.value, string + colon_index + 1, strlen(string) - colon_index);
  header.value[strlen(string) - colon_index] = '\0';

  return header;
}


int main() {
    const char* input = "Host: localhost:3000";
    Header header = split_header(input);

    char* string = "Host: localhost:3000";
    printf("String: ***%s\n", string);

    printf("Key: %s\n", header.key);
    printf("Value: %s\n", header.value);

    free(header.key);
    free(header.value);

    return 0;
}*/

#include <stdio.h>
#include <string.h>

void separateStringBySpaces(char* input) {
    // Length of the input string
    size_t length = strlen(input);

    // Create arrays to store the separated parts
    char method[length + 1];  // +1 for null terminator
    char path[length + 1];
    char protocol[length + 1];

    // Initialize the arrays
    memset(method, 0, sizeof(method));
    memset(path, 0, sizeof(path));
    memset(protocol, 0, sizeof(protocol));

    // Use sscanf to separate the input string by spaces
    sscanf(input, "%s %s %s", method, path, protocol);

    // Print the separated parts
    printf("Method: %s\n", method);
    printf("Path: %s\n", path);
    printf("Protocol: %s\n", protocol);
}

int main() {
    char input[] = "POST /hello/hi HTTP/1.1";
    separateStringBySpaces(input);
    return 0;
}

