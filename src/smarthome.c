#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "GPIO.h"

int main(void) {
	printf("OpenWRT smart home v1.00a\n");
	printf("http://russemotto.com/smarthome\n\n");

	pinMode(29, OUTPUT); // set pin 29 as output

	int sock;
	struct sockaddr_in server;
	char server_reply[2000];

	// Does a long pull on a webserver to see if there are new commands
	while(1) {
		//Create socket
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
			printf("Could not create socket");
		}		
	
		server.sin_addr.s_addr = inet_addr("/* put your ip here */");
		server.sin_family = AF_INET;
		server.sin_port = htons(80);

		// Connect
		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
		{
			perror(" Error: connection failed.");
		}
	
		printf("Connected\n");
	
		// Send the GET message to server
		char message[] = "GET /LP HTTP/1.0\r\nHost: /* put your host here */ \r\nUser-Agent: smarthome\r\n\r\n";
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
