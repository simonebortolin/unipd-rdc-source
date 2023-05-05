#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

int tmp;

struct header{
	char * n;
	char * v;
}h[100];

int main()
{
	struct sockaddr_in addr,remote_addr;
	int i,j,k,s,t,s2,len;
	char command[100];
	char * referer = NULL;

	const char * blacklist[2];
	blacklist[0] = "http://88.80.187.84:11890/page1.html";
	blacklist[1] = "http://88.80.187.84:11890/page3.html";

	int is_blacklist;
	int c;
	FILE * fin;
	int yes=1;
	char * commandline;
	char * method, *path, *ver;
	char request[5000],response[10000];
	s =  socket(AF_INET, SOCK_STREAM, 0);
	if ( s == -1 ){ perror("Socket fallita"); return 1; }
	addr.sin_family = AF_INET;
	addr.sin_port = htons(11890);
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
			else if((request[j] == ' ') && (h[k].v == request+j)){
				++h[k].v;
			}
			else if((request[j]=='\n') && (request[j-1]=='\r') ){
				request[j-1]=0;
				if(h[k].n[0]==0) break;
				h[++k].n=request+j+1;
			}
		}	

		printf("\n-----------NEW REQUEST-----------\n");
		printf("Command line = %s\n",commandline); 

		is_blacklist = 0; //reset
		
		for(i=1;i<k;i++){
			if(!strcmp(h[i].n,"Referer")){
				referer = h[i].v;
				printf("*Referer: '%s'\n",referer);
				for(int m=0;m<2;m++){ //search in the blacklist for current referer
					if(!strcmp(blacklist[m],referer)){
						is_blacklist=1;
						printf("\n*Blacklist found!");
					}
				}
			}
			printf("%s ----> %s\n",h[i].n, h[i].v);
		}
		method = commandline;
		for(i=0;commandline[i]!=' ';i++){} commandline[i]=0; path = commandline+i+1;
		for(i++;commandline[i]!=' ';i++); commandline[i]=0; ver = commandline+i+1;
		/* il terminatore NULL dopo il token versione è già stato messo dal parser delle righe/headers*/


		printf("method=%s path=%s ver=%s\n",method,path,ver);

		if(is_blacklist){ //se path e referer NON sono lo stesso
			printf("\n*FORBIDDEN");
			//sprintf(response,"HTTP/1.1 403 Forbidden\r\n\r\n");
			//is_redirect = 1;
			sprintf(response,"HTTP/1.1 301 Moved permanently\r\nLocation:%s\r\nReferrer-Policy:no-referrer\r\n\r\n",referer);
			write(s2,response,strlen(response));			
		}
		else{
			if(strncmp(path,"/cgi-bin/",9) == 0){
				sprintf(command,"%s > tmpfile.txt", path+9);
				printf("Eseguo il comando %s\n",command);
				t=system(command);
				if (t != -1)
					strcpy(path+1,"tmpfile.txt");	
			}
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

