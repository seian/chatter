#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg);
void help(char *addr);

int main(int argc, char *argv[]) {
	if(argc != 4) {
		help(argv[0]);
		exit(1);
	}

	int sockfd;
	int port;
	int chk;
	char buf[256];
	struct sockaddr_in serverAddr;
	struct hostent *server;	

	port = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0)
		error("ERROR socket open failed");
	
	server = gethostbyname(argv[1]);

	if(server == NULL) {
		fprintf(stderr, "ERROR no host");
		exit(0);
	}
	
	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
	      (char *) &serverAddr.sin_addr.s_addr,
		server->h_length);
	serverAddr.sin_port = htons(port);

	chk = connect(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	
	if(chk < 0)
		error("ERROR connection failed");

	while(1) {
		bzero(buf, 256);
		printf("you => ");
		fgets(buf, 255, stdin);
		
		if(!strncmp(buf, "qq", 2)) {
			printf("quit commander\n");
			break;
		}

		chk = write(sockfd, buf, strlen(buf));

		if(chk < 0) 
			error("ERROR on writing");
		
		bzero(buf, 256);
		chk = read(sockfd, buf, 255);

		if(chk < 0)
			error("ERROR on reading");

		printf("%s\n", buf);
	}
	
	close(sockfd);
	printf("connection close\n");

	return 0;
}

void error(char *msg) {
	perror(msg);
	exit(1);
}

void help(char *addr) {
	printf("chatter ver.0.1\n");
	printf("====================================\n");
	printf("usage : %s [ip] [port] [user name]\n", addr);
}
