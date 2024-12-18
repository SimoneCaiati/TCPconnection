#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "Server.h"

void clearwinsock(){
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char* argv[]){
	int port=PROTOPORT;
	#if defined WIN32 	//initialize winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);

	if (result != 0){
		errorhandler("Error at WSAStartup()\n");
		return 0;
	}
	#endif
	//CREATION OF THE WELCOME SOCKET
	int s_socket;
	if((s_socket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
		errorhandler("socket creation failed\n");
		clearwinsock();
		return -1;
	}
	printf("SUCCESSFUL CREATION...\n");
	//ASSOCIAZIONE DELL' INDIRIZZZO ALLA SOCKET
	struct sockaddr_in sad;
	sad.sin_family=AF_INET;
	sad.sin_addr.s_addr=inet_addr("127.0.0.1");
	sad.sin_port=htons(port);
	if(bind(s_socket,(struct sockaddr*)&sad,sizeof(sad))<0){
		errorhandler("bind() failed\n");
		closesocket(s_socket);
		clearwinsock();
		return -1;
	}
	printf("SUCCESSFUL ASSOCIATION...\n");
	//SETTING THE SOCKET TO LISTENING
	if(listen(s_socket,QLEN)<0){
		errorhandler("listen() error\n");
		closesocket(s_socket);
		clearwinsock();
		return -1;
	}
	//INFINITE LOOP OF READING AND WRITING
	struct sockaddr_in cad;
	int c_socket;
	int c_len;
	while(1){
		c_len=sizeof(cad);
 		if((c_socket=accept(s_socket,(struct sockaddr*)&cad,&c_len))<0){
			errorhandler("accept() failed\n");
			closesocket(c_socket);
			clearwinsock();
			return 0;
		}
		printf("New Connection from:%s:%d\n",inet_ntoa(sad.sin_addr),port);
		//RECEPTION OF THE MESSAGE
		char buff[BUFFSIZE];
		while (1) {
			// Initialize the receive buffer
		    memset(buff, 0, BUFFSIZE);
		    int bytes_received = recv(c_socket, buff, BUFFSIZE - 1, 0);

		    if (bytes_received <= 0) {
		    	// Client error or disconnection
		        errorhandler("client error or disconnection\n");
		        break;
		    }

		    buff[bytes_received] = '\0';

		    // Checking whether the client has sent the close command
		    if (buff[0] == 'q') {
		        printf("Client requested disconnection...\n");
		        break;
		    }

		    // Message decomposition and choice of password generation function
		    char *del = " \0"; // Delimiters
		    char *tipo = strtok(buff, del);
		    char *len = strtok(NULL, del);

		    if (!tipo || !len) {
		        continue; // Waiting for a new request without closing the socket
		    }

		    int LenNum = atoi(len); // Converts the second token to a number
		    if (LenNum <= 0) {
		        errorhandler("Invalid password length.\n");
		        continue;
		    }

		    char password[LenNum + 1];

		    // Password generation based on type
		    switch (*tipo) {
		        case 'a':
		            generate_alpha(password, LenNum);
		            break;
		        case 'n':
		            generate_numeric(password, LenNum);
		            break;
		        case 'm':
		            generate_mixed(password, LenNum);
		            break;
		        case 's':
		            generate_secure(password, LenNum);
		            break;
		        default:
		            errorhandler("Invalid password type.\n");
		            continue;
		    }

		    // Sending the generated password to the client
		    if (send(c_socket, password, strlen(password), 0) < 0) {
		        errorhandler("Password sending failed.\n");
		        break;
		    }

		    printf("Password sent correctly to client: %s\n", password);
		}
		puts("waiting for a new client...");
	}
	//AFTER THE LOOP CLOSE THE SERVER SOCKET
	closesocket(s_socket);
	clearwinsock();
	return 0;
}
