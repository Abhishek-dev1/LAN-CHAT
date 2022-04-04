#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
//#include <winsock2.h>
#include <netdb.h>
#define backlog 5
using namespace std;
static int get_size;
static int server_socket;
static int client_socket;
static string csource;

void *ftch(void *);
void *snd(void *);

int main(int argc,char *argv[])
{
	int processId, port=atoi(argv[1]);
    socklen_t len;
    struct sockaddr_in server_address, client_address;
	pthread_t procThread[3];
	//TO CHECK WETHER VALID ARGUMENTS ARE PASSED OR NOT 
	if(argc<2)
	{
		cerr<<"Not Enough Arguments"<<endl;
		return 0;
	}
	//IF THE PORT ENTERED IS VALID OR NOT 
	if(port<1024 || port>65535)
	{
		cerr<<"Enter the port number in the range (1024 - 65535)";
		return 0;
	}
	
	//CREATING A SOCKET...
	
	server_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(server_socket<0)
	{
		cerr<<"Unable to open socket"<<endl;
		return 0;
	}
	
	//INITIALIZING A SOCKET 
	
	memset(&server_address, 0, sizeof server_address);  // if not work then use  bzero((char*) &server_address, sizeof(server_address));
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=INADDR_ANY;
	server_address.sin_port = htons(port);
	
	//BINDING A SOCKET
	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address))<0)
	{
		cerr<<"Unable to bind to port"<<port<<endl;
		return 0;
	}
	
	//LISTEN TO THE PORT FOR ANY INCOMING CONNECTION
	listen(server_socket,backlog);
	int thread_no=0; // using as iterator for procthread array
	while(thread_no<3)
	{
		socklen_t len= sizeof(client_address);
		cout<<"Listening Port: "<<port<<endl;
		
		//ACCEPTING THE INCOMING CONNECTIONS
		
		client_socket=accept(server_socket,(struct sockaddr *)&client_address,&len);
		if(client_socket<0)
		{
			cerr<<"Unable to accept connection"<<endl;
			return 0;
		}
		else
		{
			csource=inet_ntoa(client_address.sin_addr); // inet_ntoa() accepts an Internet address expressed as a 32-bit quantity in network byte order and returns a string expressed in dotted-decimal notation
			cout<<"Connected to: "<<csource<<endl;
		}
		//Creating a thread for sending and receiving msg and updating the terminating condition
		pthread_create(&procThread[thread_no], NULL, ftch, NULL); 
        thread_no++;
        pthread_create(&procThread[thread_no], NULL, snd, NULL); 
        thread_no++;
	}
	// WAITING FOR A THREAD TO TERMINATE...SOMETHING LIKE A QUEUE :)
	for(int idx=0;idx<3;idx++)
	{
		pthread_join(procThread[idx],NULL);
	} 

}

void *snd(void *dummy)
{
	cout<<"Thread Num:"<<pthread_self()<<endl;
	while(true)
	{
		char msg[350];
		cout<<"\rME> ";
		memset(&msg,0,sizeof msg);   //IF NOT WORK USE bzero(msg,350)
		cin.getline(msg,350);
		
		send(client_socket,msg, strlen(msg),0);
		
		string message(msg);
		if(message=="quit")
		break;
	}
	cout<<"Closing connection..."<<endl;
	cout<<"Closed!"<<endl;
	close(client_socket);
	close(server_socket);

	exit(0);
}
void *ftch(void *dummy)
{
	cout<<"Thread Num:"<<pthread_self()<<endl;
	char msgg[350];
	memset(msgg,0,sizeof msgg);
	
	while(true)
	{
		memset(&msgg,0,sizeof msgg);
        get_size=read(client_socket, msgg, 350);
        
        string ftchmsg (msgg);
        cout <<  "\rOtherUser>"  << ftchmsg << endl;
        cout << "Me>";
	fflush(stdout);

        
	if ( ftchmsg =="quit")  break;
	else if ( get_size ==  0 ){
	cout << "\rClient Diconnected" << endl; 
	break; 
	}
	else if ( get_size == -1 ) {
	 cout << "\rRecieve Failed" << endl; 
	 break; }
	 }
	 close(client_socket);
        close(server_socket);
	 cout << "\rClosing thread and connection..." << endl;
        cout << "Closed!" << endl;
	
        
        exit(0);
}
	

