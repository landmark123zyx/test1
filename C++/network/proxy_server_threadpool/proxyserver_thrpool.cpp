#include <sys/socket.h>  
#include <resolv.h>  
#include <string.h>  
#include <thread>  
#include <future>  
#include <iostream>
#include<unistd.h>  //read()/write()
#include<netdb.h> //hostent  
#include<arpa/inet.h>  

#include <thrpool1.h>

int hostname_to_ip(char * , char *);  

 // A structure to maintain client fd, and server ip address and port address  
 // client will establish connection to server using given IP and port   
struct serverInfo  {  
  int client_fd;  
  char ip[100];  
  char port[100];  
};  

using namespace std;

 // A thread function  
 // A thread for each client request (client thread) 
void runSocket(struct serverInfo info) {  
  char buffer[65535];  
  int bytes =0;  
  cout << "client fd: " << info.client_fd << ", mainserver IP: " << info.ip
       << ", mainserver port: " << info.port << '\n';

  //code to connect to main server via this proxy server  
  int server_fd =0;  
  struct sockaddr_in server_sd;  
  // create a socket  
  server_fd = socket(AF_INET, SOCK_STREAM, 0);  
  if(server_fd < 0)  {  
    cout << "mainserver socket not created\n";
    close(info.client_fd);
    return;
  }  
  cout << "mainserver socket created\n";
  memset(&server_sd, 0, sizeof(server_sd));  
  // set socket variables  
  server_sd.sin_family = AF_INET;  
  server_sd.sin_port = htons(atoi(info.port));  
  server_sd.sin_addr.s_addr = inet_addr(info.ip);  
  //connect to main server from this proxy server  
  if((connect(server_fd, (struct sockaddr *)&server_sd, sizeof(server_sd)))<0)  {  
    cout << "mainserver connection not established\n";
    close(server_fd);
    close(info.client_fd);
    return;
  }  
  cout << "mainserver socket connected\n";

  while(1)  {  
    //receive data from client  
    memset(&buffer, '\0', sizeof(buffer));  
    bytes = read(info.client_fd, buffer, sizeof(buffer));  
    if(bytes <= 0)  {  
      cout << "proxy server: reading from client " << info.client_fd << ", error (client may be shutdown)." << '\n';
      close(server_fd);
      close(info.client_fd);
      return;
    }  
    else {  
//printf("   from client (%d bytes):\n     %x\n", bytes, buffer[0]);
      // send data to main server  
//      write(server_fd, buffer, sizeof(buffer));  
      write(server_fd, buffer, bytes);  
      //printf("client fd is : %d\n",c_fd);                    
      cout << "From client " << info.client_fd << ": ";
      fputs(buffer, stdout);       
      fflush(stdout);  
    }  
    //recieve response from server  
    memset(&buffer, '\0', sizeof(buffer));  
    bytes = read(server_fd, buffer, sizeof(buffer));  
    if(bytes <= 0)  {  
      cout << "proxy server: reading from server " << server_fd << ", error" << '\n';
      close(server_fd);
      close(info.client_fd);
      return;
    }            
    else {  
      // send response back to client  
//      write(info->client_fd, buffer, sizeof(buffer));  
      write(info.client_fd, buffer, bytes);  
      cout << "From server: ";
      fputs(buffer,stdout);            
      fflush(stdout);  
    }  
  }
}


 // main entry point  
int main(int argc,char *argv[])  {  
  char port[100],ip[100];  
  char *hostname = argv[1];  
  char proxy_port[100];  
  // accept arguments from terminal  
  strcpy(ip,argv[1]); // server ip  
  strcpy(port,argv[2]);  // server port  
  strcpy(proxy_port,argv[3]); // proxy port  
      //hostname_to_ip(hostname , ip);  
  cout << "Main server IP : " << ip << ", and port: " << port << '\n';
  cout << "Proxy server port: " << proxy_port << '\n';
  //socket variables  
  int proxy_fd =0, client_fd=0;  
  struct sockaddr_in proxy_sd;  
 // add this line only if server exits when client exits  
 signal(SIGPIPE,SIG_IGN);  
  // create a socket  
  if((proxy_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  {  
    cout << "\nFailed to create socket";
    exit(-1);
  }  
  cout << "Proxy server started\n";
  memset(&proxy_sd, 0, sizeof(proxy_sd));  
  // set socket variables  
  proxy_sd.sin_family = AF_INET;  
  proxy_sd.sin_port = htons(atoi(proxy_port));  
  proxy_sd.sin_addr.s_addr = INADDR_ANY;  
  // bind the socket  
  if((bind(proxy_fd, (struct sockaddr*)&proxy_sd,sizeof(proxy_sd))) < 0)  {  
    cout << "\nFailed to bind a socket";
    exit(-1);
  }  
  // start listening to the port for new connections  
  if((listen(proxy_fd, SOMAXCONN)) < 0)  {  
    cout << "Failed to listen" << '\n';
    exit(-1);
  }

  unsigned const thread_count=thread::hardware_concurrency(); 
//unsigned const thread_count=1;
  ThreadPool pool(thread_count);
  size_t acceptedClientNum = 0;

  cout << "waiting for connection..\n";
  //accept all client connections continuously  
  while(1) {  
    client_fd = accept(proxy_fd, (struct sockaddr*)NULL ,NULL);  
    cout << "client fd: " << client_fd << " connected\n";
    if(client_fd > 0) {  
//      acceptedClientNum++;
      acceptedClientNum = pool.getWorkingItemNumbers();
cout << "	acceptedClientNum=" << acceptedClientNum << '\n';
      if(acceptedClientNum >= thread_count) {
	cout << "Limited connection number has reached, you need to stop some previous connected channels." << '\n';
	//discard the accepted socket
	close(client_fd);
	continue;
      }

      //multithreading variables      
      struct serverInfo item;
      item.client_fd = client_fd;
      strcpy(item.ip,ip);  
      strcpy(item.port,port);  
//      pthread_create(&tid, NULL, runSocket, (void *)&item);
//      auto handle = std::async(launch::async, runSocket, &item);
      auto result = pool.enqueue(runSocket, item);
      sleep(1);  
    }  
  }  
  return 0;  
}  

 int hostname_to_ip(char * hostname , char* ip)  
 {  
   struct hostent *he;  
   struct in_addr **addr_list;  
   int i;  
   if ( (he = gethostbyname( hostname ) ) == NULL)   
   {  
     // get the host info  
     herror("gethostbyname");  
     return 1;  
   }  
   addr_list = (struct in_addr **) he->h_addr_list;  
   for(i = 0; addr_list[i] != NULL; i++)   
   {  
     //Return the first one;  
     strcpy(ip , inet_ntoa(*addr_list[i]) );  
     return 0;  
   }  
   return 1;  
 }
