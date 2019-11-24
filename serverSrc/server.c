#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define PORTNUM 1828
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

int main(){
	struct sockaddr_int saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id, sock_fd;
	FILE *sock_fp;
	
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("socket");

	bzero((void *) &saddr, sizeof(saddr));

	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);

	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET;

	if (bind(sock_id, (struct sockaddr *) &addr, sizeof(saddr)) != 0)
		oops("bind");

	if (listen(sock_id, 20) != 0)
		oops("listen");

	return 0;
}
