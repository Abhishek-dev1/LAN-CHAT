#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
using namespace std;
//Server side
int main(int argc, char *argv[])
{
	int pid;
	int newSd;
    //Check if arguments given are valid or not at command line
    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    //port no.
    int port = atoi(argv[1]);
    //msg
    if(port<1024 || port>65535)
    {
    	cerr<<"Please enter the port number in the range (1024-65535) :)"<<endl;
    	return 0;
	}
    char msg[1500];
     
    //establishing a socket
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Unable to establish the connection" << endl;
        exit(0);
    }
    //binding it to a port
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Unable to bind to a port" << endl;
        exit(0);
    }
    cout << "Listening to the port " <<port<< endl;
    //listen for up to 5 requests at a time 
    listen(serverSd, 5);
    //receive a request from client using accept
    //Address for client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    
    //session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    //bytes wrritten in sesssion
    int bytesRead, bytesWritten = 0;
    while(1)
    {
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Client request failed!" << endl;
        exit(1);
    }
    cout << "Connected to the client!" << endl;
    pid = fork();
         if (pid < 0)
             cerr<<"PID ERROR"<<endl;
         if (pid == 0)  {
             cout << "waiting for client to responsd..." << endl;
        memset(&msg, 0, sizeof(msg));//resetting the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        if(msg=="exit")
        {
            cout << "Client has quit the session" << endl;
            break;
        }
        cout << "Client: " << msg << endl;
        cout << ">";
        string data;
        getline(cin, data);
        //resseting the buffer
        memset(&msg, 0, sizeof(msg)); 
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            //send to the client that server has closed the connection
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        //send the message to client
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
         }
         
        
    }
    //we need to close the socket descriptors after we're all done
    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);
    cout << "Session Info" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) 
        << " secs" << endl;
    cout << "Connection closed..." << endl;
    return 0;   
}
