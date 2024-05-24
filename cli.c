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
//In C programming, the char* argv[] or char** argv is a common way to define the parameter that represents an array of
//strings passed to a program when it is executed from the command line.
{
	int sockfd ,portno ,n;
	struct sockaddr_in serv_addr;
	/*
In C programming, the line struct sockaddr_in servaddr, clientaddr; declares two variables: servaddr and clientaddr,
both of which are of type struct sockaddr_in.struct sockaddr_in is a structure used for handling Internet addresses in the C programming language.  
 */
	struct  hostent *server;
	// The hostent structure contains the results of a successful search for the host specified in the name parameter.
	
	char buffer[255];
	if(argc<3){
		fprintf(stderr,"usage %s hostname port \n",argv[0]);
		exit(1);
		
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	//domain -Protocol Family internet domain ,type-tcp ,protocol 0 for tcp 
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
