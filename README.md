
# Local Area Network Chat 

This command line application basically uses the user's IPv4 address and create a socket to transmit a message from one's terminal to another terminal (P-2-P Chat).


## Installation

### To run this application in Windows :-
* Download [Cygwin](https://www.cygwin.com/install.html) and then install it.
* Now search for gcc and then install gcc-g++ and gcc-core.
* After the Installation cygwin terminal get installed and then copy the files server.cpp and client.cpp and then change the directory in the terminal where the files are and then run the following commands :-
### For Server
```bash
  g++ server.cpp -o server 
  ./server 8080 
```
#### (if not worrk then use g++ server.cpp -lpthread -o server instead of above command)
### For Client
```bash
  g++ client.cpp -o client
  ./client IP_ADDRESS SERVER_PORT_NO.
```
#### (if not worrk then use g++ client.cpp -lpthread -o client instead of above command)
    
## Example

##### Server
```javascript
    g++ server.cpp -o server 
    ./server 8080
}
```
##### Client
```javascript
    g++ client.cpp -o client 
    ./client 127.0.0.1 8080
}
```
