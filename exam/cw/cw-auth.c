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
    char * statusline;
    struct sockaddr_in addr;
    int entity_length;
    int i,j,k,s,t;
    char request[5000],response[10000];
    char * entity;
    unsigned char targetip[4] = { 88,80,187,84 };
    //unsigned char targetip[4] = { 213,92,16,101 };
    s =  socket(AF_INET, SOCK_STREAM, 0);
    if ( s == -1 ){
        tmp=errno;
        perror("Socket fallita");
        printf("i=%d errno=%d\n",i,tmp);
        return 1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8553);
    addr.sin_addr.s_addr = *(unsigned int*)targetip; // <indirizzo ip del server 216.58.213.100 >

    if ( -1 == connect(s,(struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
        perror("Connect fallita");
    printf("%d\n",s);
    for(int iter=0;iter<11;iter++){
        unsigned char * auth = "apriti:sesamo";
        unsigned char encode[100];
        base64enc(auth, strlen(auth), encode);
        sprintf(request,"GET /file1.html HTTP/1.0\r\nAuthorization: Basic %s\r\n\r\n",encode);
        if ( -1 == write(s,request,strlen(request))){perror("write fallita"); return 1;}
        bzero(h,sizeof(struct header)*100);
        statusline = h[0].n=response;
        for( j=0,k=0; read(s,response+j,1);j++){
            if(response[j]==':' && (h[k].v==0) ){
                response[j]=0;
                h[k].v=response+j+1;
            }
            else if((response[j]=='\n') && (response[j-1]=='\r') ){
                response[j-1]=0;
                if(h[k].n[0]==0) break;
                h[++k].n=response+j+1;
            }
        }
        entity_length = -1;
        printf("Status line = %s\n",statusline);
        for(i=1;i<k;i++){
            if(strcmp(h[i].n,"Content-Length")==0){
                entity_length=atoi(h[i].v);
                printf("* (%d) ",entity_length);
            }
            printf("%s ----> %s\n",h[i].n, h[i].v);
        }
        if(entity_length == -1) entity_length=1000000;
        entity = (char * ) malloc(entity_length);
        for(j=0; (t=read(s,entity+j,entity_length-j))>0;j+=t);
        //if ( t == -1) { perror("Read fallita"); return 1;}
        printf("j= %d\n",j);

        for(i=0;i<j;i++) printf("%c",entity[i]);
    }
}
