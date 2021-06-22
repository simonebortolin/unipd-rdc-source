#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int tmp;

typedef struct header{
    char * n;
    char * v;
} header;

int main()
{

    FILE * fout = fopen("image.jpg", "wb");
    char * statusline;
    struct sockaddr_in addr;
    int entity_length;
    int i,j,k,s,t, ka, a, total, size;
    long tsize;
    char request[5000],response[10000];
    char * entity;
    header h[100];
    unsigned char targetip[4] = { 88,80,187,84 };
    //unsigned char targetip[4] = { 213,92,16,101 };

    a = 0;
    size = 10000;
    tsize = 10000;
    for(a=0; a<tsize; a+=size) {
        s =  socket(AF_INET, SOCK_STREAM, 0);
        if ( s == -1 ){
            tmp=errno;
            perror("Socket fallita");
            printf("i=%d errno=%d\n",i,tmp);
            return 1;
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);
        addr.sin_addr.s_addr = *(unsigned int*)targetip; // <indirizzo ip del server 216.58.213.100 >

        if ( -1 == connect(s,(struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
            perror("Connect fallita");
        printf("%d\n",s);

        sprintf(request,"GET /image.jpg HTTP/1.1\r\nRange: bytes=%d-%d\r\nHost: 88.80.187.84\r\n\r\n",a, a+size-1);
        if ( -1 == write(s,request,strlen(request))){perror("write fallita"); return 1;}
        printf("%s",request);
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
        ka = 1;
        for(i=1;i<k;i++){
            if(strcmp(h[i].n,"Content-Length")==0){
                entity_length=atoi(h[i].v);
            } else if(!strcmp(h[i].n,"Content-Range")) {
                printf("CR\n");
                int c1;
                int c2;
                int d = sscanf(h[i].v+7,"%d-%d/%ld", &c1, &c2, &tsize);
                printf("tsize %ld\n", tsize);
            }
            printf("%s ----> %s\n",h[i].n, h[i].v);
        }
        printf("%d",k);
        if(entity_length == -1) entity_length=1000000;
        entity = (char * ) malloc(entity_length);
        for(j=0; (t=read(s,entity+j,entity_length-j))>0;j+=t);
        if ( t == -1) { perror("Read fallita"); return 1;}
        fwrite(entity, entity_length,1, fout);
        //sleep(1);
        //printf("j= %d\n",j);

        //for(i=0;i<j;i++) printf("%c",entity[i]);
        close(s);
    }
    fclose(fout);
}
