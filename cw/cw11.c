#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 60000
#define HEADER_SIZE 100
#define CHUNK_SIZE 33

typedef struct header{
    char * n;
    char * v;
} header;

enum te {
    CHUNKED = 1,
    COMPRESS = 2,
    DEFLATE = 4,
    GZIP = 8,
    IDENTITY = 16
};

enum httpv {
    HTTP_0_9,
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0
};

int main()
{
    struct sockaddr_in addr; // address
    char request[BUF_SIZE], response[BUF_SIZE];
    int s = socket(AF_INET, SOCK_STREAM, 0); // socket
    header h[HEADER_SIZE]; // header
    char * rb = NULL; // response body
    char * sl = NULL; // status line
    int l = 0; // total lenght of the message
    int al; // allocate length
    int cl; // content length
    int hl; // header length
    int rl = 0; // read length
    enum te te = 0; // transfer encoding
    enum httpv httpv = 0;

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

    sprintf(request, "GET / HTTP/1.1\r\nHost:www.google.com\r\n\r\n");

    if (write(s, request, strlen(request)) == -1) {
        perror("Write fail");
        return 1;
    }

    sl = h[0].n = response;

    for (l = 0; read(s, response + l, 1); l++) {
        if (l == 8) {
            if(memcmp(response, "HTTP/", 5) != 0) {
                httpv = HTTP_0_9;
                break;
            } else if(memcmp(response, "HTTP/", 5) == 0) {
                if((response[5]=='1') && (response[7]=='0'))
                    httpv = HTTP_1_0;
                else if((response[5]=='1') && (response[7]=='1'))
                    httpv = HTTP_1_1;
                else if((response[5]=='2') && (response[7]=='0'))
                    httpv = HTTP_2_0;
            }
        }
        if ((response[l] == ':') && (h[hl].v == NULL)) {
            response[l] = 0;
            h[hl].v = response + l + 1;
        }
        else if ((response[l] == ' ') && (h[hl].v == response + l)) {
            ++h[hl].v;
        }
        else if ((response[l] == '\n') && (response[l - 1] == '\r')) {
            response[l - 1] = 0;
            if (h[hl].n[0] == 0) break;
            h[++hl].n = response + l + 1;
        }
    }

    switch (httpv) {
        case HTTP_1_0:
            cl = -1;
            for (int i = 1; i < hl; i++) {
                if (strcmp(h[i].n, "Content-Length") == 0)
                    cl = atoi(h[i].v);
            }

            if (cl == -1) {
                cl = BUF_SIZE;
            }

            rb = (char *) malloc(cl + 1); // +1 is for the string cap

            for (int j = 0; (j = read(s, rb + rl, cl - rl)) > 0; rl += j);

            rb[rl] = 0;
        break;
        case HTTP_1_1:
            cl = -1;

            for (int i = 1; i < hl; i++) {
                if (strcmp(h[i].n, "Transfer-Encoding") == 0) {
                    char * token = strtok(h[i].v, ", ");
                    while( token != NULL ) {
                        if(strcasecmp(token, "chunked") == 0) {
                            te |= CHUNKED;
                        } else if(strcasecmp(token, "compress") == 0) {
                            te |= COMPRESS;
                        } else if(strcasecmp(token, "deflate") == 0) {
                            te |= DEFLATE;
                        } else if(strcasecmp(token, "gzip") == 0) {
                            te |= GZIP;
                        } else if(strcasecmp(token, "x-gzip") == 0) {
                            te |= GZIP;
                        } else if(strcasecmp(token, "identity") == 0) {
                            te |= IDENTITY;
                        }

                        token = strtok(NULL, " ");
                    }
                }
                else if (strcmp(h[i].n, "Content-Length") == 0)
                    cl = atoi(h[i].v);
                printf("%s ----> %s\n", h[i].n, h[i].v);
            }
            // It is important that in phase of reading of the body of the message it is given precedence to the
            // Transfer-Encoding regarding the Content-Length
            // See RFC 2616
            if(te & CHUNKED) {
                al = BUF_SIZE;
                rb = malloc(al);
                char chs[CHUNK_SIZE];
                char trailer[CHUNK_SIZE];
                bzero(chs,CHUNK_SIZE);
                for (int j = 0; (read(s, chs + j, 1)) && !((chs[j] == '\n') && (chs[j - 1] == '\r')); j++);

                cl = (int)strtol(chs, NULL, 16);
                while(cl > 0) {
                    if(rl + cl + 1 > al) {
                        rb = realloc(rb, al *= 2 );
                    }
                    for (int j = 0, ll = 0; (j = read(s, rb + rl, cl - ll)) > 0; rl += j, ll += j);

                    for (int j = 0; (read(s, trailer + j, 1)) && !((trailer[j] == '\n') && (trailer[j - 1] == '\r')); j++);

                    bzero(chs,CHUNK_SIZE);

                    for (int j = 0; (read(s, chs + j, 1)) && !((chs[j] == '\n') && (chs[j - 1] == '\r')); j++);

                    cl = (int)strtol(chs, NULL, 16);
                }
                rb[rl] = 0;
            } else if(cl > -1) {
                rb = (char *) malloc(cl + 1); // +1 is for the string cap

                for (int j = 0; (j = read(s, rb + rl, cl - rl)) > 0; rl += j);

                rb[rl] = 0;
            }
            else if(te & IDENTITY || cl != -1) {
                rb = (char *) malloc(cl + 1); // +1 is for the string cap

                for (int j = 0; (j = read(s, rb + rl, cl - rl)) > 0; rl += j);

                rb[rl] = 0;
            }

        break;
        default:
            break;
    }
    printf("Status line = %s\n", sl);

    printf("%s", rb);

    printf("\nRead %d chars", rl);

    free(rb);
}