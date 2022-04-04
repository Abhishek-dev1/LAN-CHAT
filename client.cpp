#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
//#include <winsock2.h>
using namespace std;

void *ftch(void *);
void *snd(void *);
static int client_socket;
static int get_size;

int main(int argc,char *argv[])
{
	int port;
    struct sockaddr_in server_address;
    struct hostent *server;
	pthread_t procThread[3];
	//TO CHECK WETHER VALID ARGUMENTS ARE PASSED OR NOT 
	if(argc<3)
	{
		cerr<<"Not Enough Arguments:( Syntax: ./nameclient <hostName> <port>"<<endl;
		return 0;
	}
	port=atoi(argv[2]);
	//IF THE PORT ENTERED IS VALID OR NOT 
	if(port<1024 || port>65535)
	{
		cerr<<"Enter the port number in the range (1024 - 65535)";
		return 0;
	}
	//CREATING A SOCKET...
	
	client_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(client_socket<0)
	{
		cerr<<"Unable to open socket"<<endl;
		return 0;
	}
	server=gethostbyname(argv[1]); //returns a pointer to the hostent structure containing the host's IP address and other information
	if(server==NULL)
	{
		cerr<<"Unable to find the host"<<endl;
		return 0;
	}
	//INITIALIZING A SOCKET 
	
	memset(&server_address, 0, sizeof server_address);  // if not work then use  bzero((char*) &server_address, sizeof(server_address));
	server_address.sin_family=AF_INET;
	bcopy((char *) server -> h_addr, (char *) &server_address.sin_addr.s_addr, server -> h_length); //A NULL-terminated list of addresses for the host
																								    //Addresses are returned in network byte order
																								    // The macro h_addr is defined to be h_addr_list[0] for compatibility
	server_address.sin_port =htons(port);
	//COnNECTIONG TO THE SERVER...
	int connection=connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
	
	if(connection < 0)
	{
		cerr<<"Connection Error! Unable to connect"<<endl;
		return 0;
	}
	int thread_no=0; // using as iterator for procthread array
	while(thread_no<3)
	{
		
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

	exit(0);
}
void *ftch(void *dummy)
{
	char msgg[350];
	memset(&msgg,0,sizeof msgg);
	
	while(true)
	{
		memset(&msgg,0,sizeof msgg);
        get_size=read(client_socket, msgg, 350);
        
        string ftchmsg (msgg);
        cout <<  "\rOtherUser> "  << ftchmsg << endl;
        cout << "Me> ";
	fflush(stdout);
        
	if ( ftchmsg =="quit")  break;
	else if ( get_size ==  0 ){
	cout << "\rServer Diconnected" << endl; 
	break; 
	}
	else if ( get_size == -1 ) {
	 cout << "\rRecieve Failed" << endl; 
	 break; 
	 }
}
        cout << "\nClosing thread and connection..." << endl;
        cout << "Closed!" << endl;
        close(client_socket);
        exit(0);
}