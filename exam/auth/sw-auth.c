#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>      /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

int tmp;

struct header{
    char * n;
    char * v;
}h[100];

unsigned char alphabet[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz"
                    "0123456789+/";


unsigned char decodingtable[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};

int base64enc(char* in, int len, char* to){
    int i, j;
    unsigned char * o,* s;

    for(i=0, j=0; i<len; i+=3, j+=4)
    {

        o = in + i;
        s = to + j;

        s[0] = alphabet[o[0] >> 2];

        printf("%d ", len-i);
        if (len-i == 1)
        {
            s[1] = alphabet[(o[0] & 0x3) << 4];
            s[2] = s[3] = '=';
        }
        else if (len -i == 2)
        {
            s[1] = alphabet[((o[0] & 0x3) << 4) | (o[1] >> 4)];
            s[2] = alphabet[(o[1] & 0xF) << 2];
            s[3] = '=';
        }
        else{
            s[1] = alphabet[((o[0] & 0x3) << 4) | (o[1] >> 4)];
            s[2] = alphabet[((o[1] & 0xF) << 2) | (o[2] >> 6)];
            s[3] = alphabet[o[2] & 0x3F];
        }
    }
    return j+1;
}


int base64dec(const unsigned char* in, int len, unsigned char* to) {
    int i, j;
    int pad = len > 0 && (len % 4 || in[len - 1] == '=');
    int ll = ((len + 3) / 4 - pad) * 4;
    const unsigned char * o;
    unsigned char *s;
    for(i=0, j=0; j<ll; j+=4, i+=3) {
        o = in + j;
        s = to + i;
        s[0] = (decodingtable[o[0]] << 2) | ((decodingtable[o[1]] & 0x30) >> 4);
        s[1] = ((decodingtable[o[1]] & 0xF) << 4) | ((decodingtable[o[2]] & 0x3C) >> 2);
        s[2] = ((decodingtable[o[2]] & 0x3) << 6) | decodingtable[o[3]];

    }
    o = in + j;
    s = to + i;
    if(pad) {
        if(in[len-2] != '=') {
            s[0] = (decodingtable[o[0]] << 2) | ((decodingtable[o[1]] & 0x30) >> 4);
            s[1] = ((decodingtable[o[1]] & 0xF) << 4) | ((decodingtable[o[2]] & 0x3C) >> 2);
            s[2] = ((decodingtable[o[2]] & 0x3) << 6);
            pad = 3;
        } else {
            s[0] = (decodingtable[o[0]] <<2 ) | ((decodingtable[o[1]] & 0x30) >> 4);
            s[1] = ((decodingtable[o[1]] & 0xF) << 4);
            pad = 2;
        }
    }
    int sl = ll/4*3+pad;

    to[sl]=0;
    return sl;
}

int main()
{
    struct sockaddr_in addr,remote_addr;
    int i,j,k,s,t,s2,len, count;
    char command[100];
    int c;
    FILE * fin;
    int yes=1;
    char * commandline;
    char * method, *path, *ver;
    char * encoded;
    char request[5000],response[10000];
    s =  socket(AF_INET, SOCK_STREAM, 0);
    if ( s == -1 ){ perror("Socket fallita"); return 1; }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8553);
    addr.sin_addr.s_addr = 0;
    t= setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    if (t==-1){perror("setsockopt fallita"); return 1;}
    if ( bind(s,(struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {perror("bind fallita"); return 1;}
    if ( listen(s,225) == -1 ) { perror("Listen Fallita"); return 1; }
    len = sizeof(struct sockaddr_in);
    while(1){
        s2 =  accept(s, (struct sockaddr *)&remote_addr,&len);
        if ( s2 == -1 ) { perror("Accept Fallita"); return 1;}
        bzero(h,100*sizeof(struct header *));
        commandline = h[0].n=request;
        for( j=0,k=0; read(s2,request+j,1);j++){
            if(request[j]==':' && (h[k].v==0) ){
                request[j]=0;
                h[k].v=request+j+1;
            }
            else if((request[j]=='\n') && (request[j-1]=='\r') ){
                request[j-1]=0;
                if(h[k].n[0]==0) break;
                h[++k].n=request+j+1;
            }
        }
        printf("Command line = %s\n",commandline);

        count = 0;

        for(i=1;i<k;i++){
            printf("%s ----> %s\n",h[i].n, h[i].v);
            //controlla se si tratta dell'header con le credenziali
            if(strcmp(h[i].n,"Authorization") == 0) {
                if(strncmp(h[i].v," Basic",6) == 0){
                    encoded = "apriti:sesamo";
                    char decode[100];
                    base64dec(h[i].v+7, strlen(h[i].v+7), decode);
                    count = !strcmp(encoded, decode);


                }
            }
        }
        //se count == 0 richiediamo l'autenticazione
        if(!count)
        {
            sprintf(response, "HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: Basic realm='boh'\r\n\r\n");
            write(s2,response,strlen(response));
        } else {
            method = commandline;
            for(i=0;commandline[i]!=' ';i++){} commandline[i]=0; path = commandline+i+1;
            for(i++;commandline[i]!=' ';i++); commandline[i]=0; ver = commandline+i+1;
            /* il terminatore NULL dopo il token versione è già stato messo dal parser delle righe/headers*/

            printf("method=%s path=%s ver=%s\n",method,path,ver);

            // esegue la richiesta dopo essersi accertato di essere autenticato
            //caso cgi-bin
            if(strncmp(path,"/cgi-bin/",9) == 0){
                sprintf(command,"%s > tmpfile.txt", path+9);
                printf("Eseguo il comando %s\n",command);
                t=system(command);
                if (t != -1)
                    strcpy(path+1,"tmpfile.txt");
            }
            //caso normale
            if ((fin = fopen(path+1,"rt"))==NULL){
                sprintf(response,"HTTP/1.1 404 Not Found\r\n\r\n");
                write(s2,response,strlen(response));
            }
            else {
                sprintf(response,"HTTP/1.1 200 OK\r\n\r\n");
                write(s2,response,strlen(response));
                while ( (c = fgetc(fin)) != EOF )
                    write(s2,&c,1);
                fclose(fin);
            }
        }
        close(s2);
    }
}
