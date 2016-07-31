#include <sys/socket.h>  
#include <resolv.h>  //struct sockaddr_in
#include <string.h> //strcpy()
#include <unistd.h> //read()/write()
#include <iostream>

using namespace std;

 // main entry point  
int main(int argc, char* argv[])  {  
  //socket variables  
  char IP[200];  
  char port[32];  
  char buffer[65535];  
  int sd;  
  struct sockaddr_in client_sd;  

  if(argc != 3) {
    cout << "usage: " << argv[0] << " <proxy's IP> <proxy's port>\n";  
    exit(-1);
  } 
  strcpy(IP, argv[1]);
  strcpy(port, argv[2]);
  
  // create a socket  
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  {  
    cout << "socket not created\n";
    exit(-1);
  }  
  memset(&client_sd, 0, sizeof(client_sd));  
  // set socket variables  
  client_sd.sin_family = AF_INET;  
  client_sd.sin_port = htons(atoi(port));  
  // assign any IP address to the client's socket  
  client_sd.sin_addr.s_addr = INADDR_ANY;   
  // connect to proxy server at mentioned port number  
  connect(sd, (struct sockaddr *)&client_sd, sizeof(client_sd));  
  //send and receive data contunuously  
  int i = 0;
  char s[1];
  while(1) {  
    errno = 0; //init no error
    cout << "Type here:";
    fgets(buffer, sizeof(buffer), stdin);
//    write(sd, buffer, strlen(buffer));  
    i = write(sd, buffer, strlen(buffer));
    if(i==-1) {
      //should use strerror_r() for thread-safe
      cout << "write to proxy server error: " << strerror(errno) << '\n';
      break;
    } 
    cout << "Server response: ";
//    read(sd, buffer, sizeof(buffer));  
    i = 0;
    while(1) {
      read(sd, s, 1);
//printf("  s=%x\n", s[0]); 
      if(s[0] != '\r' && s[0] != '\n' && s[0] != 0)
	buffer[i++] = s[0];
      else
	break;
    }
    buffer[i] = '\n';
    buffer[i+1] = '\0';
    fputs(buffer, stdout);  
    //cout << endl;
  }  
  close(sd);  
  return 0;  
}
