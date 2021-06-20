#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>      /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

int tmp;
struct header {
    char *n;
    char *v;
} h[100];

int main() {
    char *statusline;
    struct sockaddr_in addr;
    int entity_length;
    int is_cl;
    int i, j, k, s, t, jj;
    char request[5000], response[10000];
    char *entity;
    unsigned char targetip[4] = { 142,250,186,78 };
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        tmp = errno;
        perror("Socket fallita");
        printf("i=%d errno=%d\n", i, tmp);
        return 1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = *(unsigned int*) targetip;

    if (connect(s, (struct sockaddr*) &addr, sizeof(struct sockaddr_in)) == -1)
        perror("Connect fallita");
    printf("%d\n", s);
    sprintf(request, "GET /pippo HTTP/1.0\r\n\r\n");
    if (write(s, request, strlen(request)) == -1 ) {
        perror("write fallita");
        return 1;
    }
    bzero(h,sizeof(struct header)*100);
    statusline = h[0].n = response;
    for (j = 0, k = 0; read(s, response + j, 1); j++) {
        if (response[j] == ':' && (h[k].v == 0)) {
            response[j] = 0;
            h[k].v = response + j + 1;
        } else if ((response[j] == '\n') && (response[j - 1] == '\r')) {
            response[j - 1] = 0;
            if (h[k].n[0] == 0)
                break;
            h[++k].n = response + j + 1;
        }
    }
    entity_length = -1;
    printf("Status line = %s\n", statusline);
    is_cl = 0;
    for (i = 1; i < k; i++) {
        if (strcmp(h[i].n, "Content-Length") == 0) {
            entity_length = atoi(h[i].v);
            is_cl=1;
        }

    }
    if (entity_length == -1)
        entity_length = 1000000;
    entity = (char*) malloc(entity_length);

    if(is_cl) {
        for(j=0; read(s,entity+j,entity_length-j)>0;j+=t);
    }

    for (i = 0; i < entity_length; i++)
        printf("%c", entity[i]);

    free(entity);
    
}