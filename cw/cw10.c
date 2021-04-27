#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 60000
#define HEADER_SIZE 100

typedef struct header{
    char * n;
    char * v;
} header;

int main()
{
    struct sockaddr_in addr; // address
    char request[BUF_SIZE], response[BUF_SIZE];
    int s = socket(AF_INET, SOCK_STREAM, 0); // socket
    header h[HEADER_SIZE]; // header
    char * rb = NULL; // response body
    char * sl; // status line
    int cl; // contect length
    int hl; // header length
    int rl; // read length

    if (s == -1) {
        perror("Socket fail");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("216.58.213.100"); // Google server

    if (connect(s, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        perror("Connect fail");
        return 1;
    }

    sprintf(request, "GET /pippo HTTP/1.0\r\n\r\n");

    if (write(s, request, strlen(request)) == -1) {
        perror("Write fail");
        return 1;
    }

    sl = h[0].n = response;

    for (int j = 0; read(s, response + j, 1); j++) {
        if ((response[j] == ':') && (h[hl].v == 0)) {
            response[j] = 0;
            h[hl].v = response + j + 1;
        }
        else if ((response[j] == ' ') && (h[hl].v = response + j)) {
            ++h[hl].v;
        }
        else if ((response[j] == '\n') && (response[j - 1] == '\r')) {
            response[j - 1] = 0;
            if (h[hl].n[0] == 0) break;
            h[++hl].n = response + j + 1;
        }
    }

    printf("Status line = %s\n", sl);

    cl = -1;
    for (int i = 1; i < hl; i++) {
        if (strcmp(h[i].n, "Content-Length") == 0)
            cl = atoi(h[i].v);

        printf("%s ----> %s\n", h[i].n, h[i].v);
    }

    if (cl == -1) {
        cl = BUF_SIZE;
    } else {
        printf("Content-Length: %d\n", cl);
    }

    printf("Malloc-Size: %d\n", cl+1);

    rb = (char *) malloc(cl + 1); // +1 is for the string cap

    for (int j = 0; (j = read(s, rb + j, cl - j)) > 0; rl += j);

    rb[rl] = 0;

    printf("%s", rb);


    printf("\nRead-Length: %d", rl);

    free(rb);
}