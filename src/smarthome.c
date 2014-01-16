#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include "GPIO.h"

int main(int argc, char *argv[]) {

	char *ip;
	char *host;
	int sock;
	struct sockaddr_in server;
	char server_reply[2000];

	printf("OpenWRT smart home v1.00a\n");
	printf("http://smarthome.russemotto.com\n\n");

	if(argc <2)
	{
		printf("\nI need a hostname\n");
		return 0;
	}

	// host name to ip
	host = argv[1];
	struct hostent *h;
	if ((h=gethostbyname(host)) == NULL) {  // get the host info
		herror("gethostbyname");
		return 0;
	}
	ip = inet_ntoa(*((struct in_addr *)h->h_addr));

	pinMode(29, OUTPUT); // set pin 29 as output

	// Does a long pull on a webserver to see if there are new commands
	while(1) {
		//Create socket
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
			printf("Could not create socket");
		}		
	
		server.sin_addr.s_addr = inet_addr(ip);
		server.sin_family = AF_INET;
		server.sin_port = htons(80);

		// Connect
		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
		{
			perror(" Error: connection failed.");
		}
	
		printf("Connected\n");
	
		// Send the GET message to server
		char *message = malloc(26+strlen(host)+28);   
		strcpy(message, "GET /LP HTTP/1.0\r\nHost: ");
		strcat(message, host);
		strcat(message, "\r\nUser-Agent: smarthome\r\n\r\n");
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			printf("Error: Send failed");			
		}
		
		//Wait for replay from server
		if( recv(sock , server_reply , 2000 , 0) >= 0)
		{		
			puts("Server reply :");
			puts(server_reply);

			int htmlstart = 0;
  			char * htmlcontent;

			if(htmlstart == 0)
			{
      				htmlcontent = strstr(server_reply, "\r\n\r\n");
				if(htmlcontent != NULL){
				htmlstart = 1;
				htmlcontent += 4;
			}
    		}

		
		// Check if there was a command for us
		if (strncmp(htmlcontent, "on", 2) == 0) {
			printf("Turning on");
			turnOn(2);	
		} 

		if (strncmp(htmlcontent, "off", 3) == 0) {
			printf("Turning off");
			turnOff(2); 
		} 
		}
	close(sock);
	}
		
	return 0;
}
