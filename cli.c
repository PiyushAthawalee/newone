#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<netdb.h>
void error(const char *msg)
{
	perror(msg);
	exit(1);
}
int main(int argc ,char *argv[])
{
	int sockfd ,portno ,n;
	struct sockaddr_in serv_addr;
	struct  hostent *server;
	
	char buffer[255];
	if(argc<3){
		fprintf(stderr,"usage %s hostname port \n",argv[0]);
		exit(1);
		
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd <0)
		error("error opening socket");
		
		server = gethostbyname(argv[1]);
		if(server == NULL)
		{
		fprintf(stderr ,"error no such host");
		}
		bzero((char *) &serv_addr , sizeof(serv_addr));
		serv_addr.sin_family =AF_INET;
		bcopy((char *) server ->h_addr ,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
		serv_addr.sin_port =htons(portno);
		if(connect(sockfd , (struct sockaddr *) &serv_addr ,sizeof(serv_addr))<0)
			error("connection fail");
		
		while(1)
		{
			bzero(buffer ,255);
			fgets(buffer ,255 ,stdin);
			n = write(sockfd ,buffer,strlen(buffer));
			if (n<0)
				error("error w");
			bzero(buffer ,255);
			n=read(sockfd ,buffer,255);
			if(n<0)
				error("error w");
			printf("server :%s",buffer);
			int i = strncmp("bye",buffer,3);
		 	if (i==0)
		 	break;
				
		
		}	
	close(sockfd);
	return 0;
		
		
		
			
	
	
	
	
}
