#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[255];
    fd_set read_fds;

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

    // Prompt for the client's name and send it to the server
    printf("Enter your name: ");
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
   
    
    buffer[strcspn(buffer, "\n")] = '\0';  // Remove the newline character
    
   
    n = write(sockfd, buffer, strlen(buffer));
    
    if (n < 0)
        error("Error writing to socket");

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(sockfd, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        if (select(sockfd + 1, &read_fds, NULL, NULL, NULL) == -1)
            error("Select error");

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            bzero(buffer, 255);
            fgets(buffer, 255, stdin);
          
            n = write(sockfd, buffer, strlen(buffer));
            
    
            if (n < 0)
                error("Error writing to socket");

            int i = strncmp("bye", buffer, 3);
            if (i == 0)
                break;
        }

        if (FD_ISSET(sockfd, &read_fds)) {
            bzero(buffer, 255);
            n = read(sockfd, buffer, 255);
            if (n < 0)
                error("Error reading from socket");
            printf("%s\ntype..", buffer);
        }
    }

    close(sockfd);
    return 0;
}
