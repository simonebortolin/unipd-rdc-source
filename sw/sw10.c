#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define BUF_SIZE 60000
#define HEADER_SIZE 100
#define CHUNK_SIZE 33

typedef struct header {
    char *n;
    char *v;
} header;

volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}

int main() {
    struct sockaddr_in addr; // address
    struct sockaddr_in remote_addr; // remote address
    char request[BUF_SIZE], response[BUF_SIZE];
    int s = socket(AF_INET, SOCK_STREAM, 0);
    header h[HEADER_SIZE]; // header

    int i, j, k, t, len;
    int c;
    FILE *fin;
    int yes = 1;
    char *commandline;
    char *method, *path, *ver;

    if (s == -1) {
        perror("Socket fail");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8053);
    addr.sin_addr.s_addr = 0;

    t = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (t == -1) {
        perror("setsockopt fallita");
        return 1;
    }
    if (bind(s, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        perror("bind fallita");
        return 1;
    }
    if (listen(s, 5) == -1) {
        perror("Listen Fallita");
        return 1;
    }
    len = sizeof(struct sockaddr_in);

    signal(SIGINT, inthand);

    while (!stop) {
        int sc = accept(s, (struct sockaddr *) &remote_addr, &len);
        if (sc == -1) {
            perror("Accept Fallita");
            return 1;
        }
        bzero(h, 100 * sizeof(struct header *));
        commandline = h[0].n = request;
        for (j = 0, k = 0; read(sc, request + j, 1); j++) {
            if (request[j] == ':' && (h[k].v == 0)) {
                request[j] = 0;
                h[k].v = request + j + 1;
            } else if ((request[j] == '\n') && (request[j - 1] == '\r')) {
                request[j - 1] = 0;
                if (h[k].n[0] == 0) break;
                h[++k].n = request + j + 1;
            }
        }
        printf("Command line = %s\n", commandline);
        for (i = 1; i < k; i++) {
            printf("%s ----> %s\n", h[i].n, h[i].v);
        }
        method = commandline;
        for (i = 0; commandline[i] != ' '; i++) {}
        commandline[i] = 0;
        path = commandline + i + 1;
        for (i++; commandline[i] != ' '; i++);
        commandline[i] = 0;
        ver = commandline + i + 1;
        for (i++; commandline[i] != '\r' && commandline[i] != 0; i++);
        commandline[i] = 0;
        printf("method=%s path=%s ver=%s\n", method, path, ver);

        if ((fin = fopen(path + 1, "rt")) == NULL) {
            sprintf(response, "HTTP/1.0 404 Not Found\r\n\r\n");
            write(sc, response, strlen(response));
        } else {
            sprintf(response, "HTTP/1.0 200 OK\r\n\r\n");
            write(sc, response, strlen(response));
            while ((c = fgetc(fin)) != EOF)
                write(sc, &c, 1);
            fclose(fin);
        }
        close(sc);
    }
}
