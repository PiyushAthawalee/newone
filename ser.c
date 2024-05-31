#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Port not provided\n");
        exit(1);
    }

    int sockfd, newsockfd, portno;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    fd_set read_fds, temp_fds;
    int fdmax;

    char client_names[FD_SETSIZE][16];  // array to store client names

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Binding failed");
    }

    listen(sockfd, 5);
    prientf("socket is open:");

    FD_ZERO(&read_fds);
    FD_ZERO(&temp_fds);
    FD_SET(sockfd, &read_fds);
    fdmax = sockfd;

    while (1) {
        temp_fds = read_fds;

        if (select(fdmax + 1, &temp_fds, NULL, NULL, NULL) == -1) {
            error("Select error");
        }

        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &temp_fds)) {
                if (i == sockfd) {
                    clilen = sizeof(cli_addr);
                    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
                    if (newsockfd < 0) {
                        error("Error on accept");
                    } else {
                        FD_SET(newsockfd, &read_fds);
                        if (newsockfd > fdmax) {
                            fdmax = newsockfd;
                        }
                        bzero(buffer, 255);
                        read(newsockfd, buffer, 255);  // read client's name
                        strncpy(client_names[newsockfd], buffer, 16);
                        printf("New connection from %s on socket %d\n", client_names[newsockfd], newsockfd);
                    }
                } else {
                    bzero(buffer, 255);
                    int n = read(i, buffer, 255);
                    if (n <= 0) {
                        if (n == 0) {
                            printf("%s (socket %d) hung up\n", client_names[i], i);
                        } else {
                            error("Error reading from socket");
                        }
                        close(i);
                        FD_CLR(i, &read_fds);
                    } else {
                        char message[300];
                        snprintf(message, 300, "%s: %s", client_names[i], buffer);

                        for (int j = 0; j <= fdmax; j++) {
                            if (FD_ISSET(j, &read_fds) && j != sockfd && j != i) {
                                if (write(j, message, strlen(message)) < 0) {
                                    error("Error writing to socket");
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
