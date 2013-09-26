#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg);
void help(char *addr);

int main(int argc, char *argv[]) {
	if(argc != 3) {
		help(argv[0]);
		exit(1);
	}
		
	int sockfd;
	int newsockfd;
	int port;
	int chk;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	char buf[256];
	socklen_t clilen;
	
	printf("socket establishing...\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0)
		error("ERROR socket open failed");

	port = atoi(argv[1]);
	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family		= AF_INET;
	serverAddr.sin_addr.s_addr	= INADDR_ANY;
	serverAddr.sin_port		= htons(port);
	chk = bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	
	if(chk < 0)
		error("ERROR bind failed");

	listen(sockfd, 5);
	clilen = sizeof(clientAddr);
	newsockfd = accept(sockfd, (struct sockaddr *) &clientAddr, &clilen);
	printf("connected!\n");

	if(newsockfd < 0)
		error("ERROR acception failed");

	while(1) {
		bzero(buf, 256);
		chk = read(newsockfd, buf, 255);
		
		if(chk < 0)
			error("ERROR connection failed");

		printf("%s\n", buf);
		printf("you => ");
		bzero(buf, 256);
		fgets(buf, 255, stdin);
		
		if(!strncmp(buf, "qq", 2)) {
			printf("quit command\n");
			break;
		}

		chk = write(newsockfd, buf, strlen(buf));
		
		if(chk < 0)
			error("ERROR on writing");

	}
	
	close(newsockfd);
	close(sockfd);


	return 0;
}

void error(char *msg) {
	perror(msg);
	exit(1);
}

void help(char *addr) {
	printf("Chatter V.0.1 BETA\n");
	printf("============================\n");
	printf("USAGE: %s <port> <user name>\n", addr);
}
