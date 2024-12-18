#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "Client.h"

void clearwinsock(){
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char* argv[]){
	int port=PROTOPORT;
#if defined WIN32 //inizialize the winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);

	if (result != 0){
		errorhandler("Error at WSAStartup()\n");
		return 0;
	}
	#endif
	//CREATION OF THE CLIENT SOCKET
	int c_socket;
	if((c_socket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
		errorhandler("Client socket creation failed\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	printf("SUCCESSFUL CREATION...\n");
	//CREATION OF SERVER ADDRESS AND CONNECTION
	struct sockaddr_in sad;
	memset(&sad,0,sizeof(sad));
	sad.sin_family=AF_INET;
	sad.sin_addr.s_addr=inet_addr("127.0.0.1");
	sad.sin_port=htons(port);
	//CONNECTION
	if(connect(c_socket,(struct sockaddr*)&sad,sizeof(sad))<0){
		errorhandler("connect() failed\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	printf("Successful connection...\n");
	//READING AND WRITING
	char messaggio[10];
	do{
		menu();
		char errorstring[50];
		printf("\nchoose the message:\n");
		fgets(messaggio,9,stdin);
		if(msgValidate(messaggio,errorstring))
		{
			printf("correct insertion...\n");
			//SENDING THE MESSAGE TO THE SERVER
			if(send(c_socket,messaggio,sizeof(messaggio),0)<0){
				errorhandler("sending the message failed\n");
				break;
			}
			else printf("message sent correctly to the server...\n");

			//RECEPTION OF RESPONSE
			char buf[BUFFSIZE];
			memset(buf, 0, BUFFSIZE);

			if(recv(c_socket,buf,BUFFSIZE-1,0)<=0){
				errorhandler("recv() failed");
				break;
			}
			else printf("Password recieved: %s\n",buf);
		}
		else printf("%s...\n\n",errorstring);

	}while(messaggio[0]!='q');
	//CLOSE CONNECTION
	closesocket(c_socket);
	clearwinsock();
	printf("\n");
	system("pause");
	return 0;
}
