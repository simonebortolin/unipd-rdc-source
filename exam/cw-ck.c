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
    int is_ck;
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
    for (int iter = 0; iter < 10; iter++) {
        sprintf(request, "GET / HTTP/1.1\r\nConnection:keep-alive\r\nHost:www.google.com\r\n\r\n");
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
        is_ck = 0;
        is_cl = 0;
        for (i = 1; i < k; i++) {
            if (strcmp(h[i].n, "Content-Length") == 0) {
                entity_length = atoi(h[i].v);
                is_cl=1;
            }
            else if(strcmp(h[i].n, "Transfer-Encoding") == 0) {
                if(strcmp(h[i].v, " chunked") == 0){
                    is_ck = 1;
                }
            }

        }
        if (entity_length == -1)
            entity_length = 1000000;
        entity = (char*) malloc(entity_length);

        if(is_ck){
            entity_length = 0;
            int chunk_length = 0, row_end = 1, cp = 0, is_valid = 0, rl = 0, ep = 0;
            char *chunk_size = malloc(100);
            do{
                for(jj=0; read(s, (chunk_size + jj), 1) > 0; ++jj){
                    if((jj > 0 ) && (chunk_size[jj] == '\n' ) && (chunk_size[jj - 1] == '\r' ) ){
                        chunk_size[jj - 1] = 0;
                        break;
                    }
                }
                int digit = 0;
                chunk_length = 0;
                if(strlen(chunk_size) > 0)
                    chunk_length = (int)strtol(chunk_size,NULL, 16);


                entity_length = entity_length + chunk_length;
                entity = (char*) realloc(entity,entity_length);

                for(jj=0; (t = read(s, entity + ep + jj, chunk_length - jj ) ); jj+=t);

                ep += jj;
                read(s, chunk_size,1);
                read(s, chunk_size+1,1);

                if( ( chunk_size[0] != '\r' ) || ( chunk_size[1] != '\n' ) ){
                    printf("Errore di parsing. Mancano CR e LF.\n");
                }

            } while(chunk_length > 0);
            free(chunk_size);

        }
        else if(is_cl) {
            for(j=0; read(s,entity+j,entity_length-j)>0;j+=t);
        }

        for (i = 0; i < entity_length; i++)
            printf("%c", entity[i]);

        free(entity);
    }
}
