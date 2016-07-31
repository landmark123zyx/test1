#include <sys/socket.h>  
#include <resolv.h>  
#include <string.h>  
#include<unistd.h> 
#include <thread>  
#include <future>
#include <iostream>

#include <thrpool1.h>

using namespace std;

 // A thread function  
 // A thread is created for each accepted client connection  
//prx_fd: proxy fd
void runSocket(int prx_fd) {  
  char buffer[65535];  
  int bytes = 0;  
  while(1)  {  
    //receive data from client  
    memset(&buffer,'\0',sizeof(buffer));  
    bytes = read(prx_fd, buffer, sizeof(buffer));  
    if(bytes <=0)  {  
      cout << "read from proxy: error" << '\n';
      return;
    } else  {  
      //send the same data back to client  
      // similar to echo server  
      write(prx_fd, buffer, bytes);  
      //printf("client fd is : %d\n",c_fd);                    
      //printf("From client:\n");                    
      fputs(buffer,stdout);       
    }  
    fflush(stdout);  
  }       
}  

int main(int argc, char* argv[]) {  
  int proxy_fd;  
  char buffer[100], port[32];  
  int fd = 0 ;  
  struct sockaddr_in server_sd;  

  if(argc != 2) {
    cout << "usage: " << argv[0] << " <port>" << '\n';
    exit(-1);    
  }
  strcpy(port, argv[1]);

 // add this line only if server exits when client exits  
 signal(SIGPIPE,SIG_IGN);  
  // create a socket  
  fd = socket(AF_INET, SOCK_STREAM, 0);  
  cout << "Main server started" << '\n';  
  memset(&server_sd, 0, sizeof(server_sd));  
  // set socket variables  
  server_sd.sin_family = AF_INET;  
  server_sd.sin_port = htons(atoi(port));  
  server_sd.sin_addr.s_addr = INADDR_ANY;  
  // bind socket to the port  
  bind(fd, (struct sockaddr*)&server_sd, sizeof(server_sd));  
  // start listening at the given port for new connection requests  
  listen(fd, SOMAXCONN);  

  unsigned const thread_count=thread::hardware_concurrency();
//unsigned const thread_count=1;
  ThreadPool pool(thread_count);
  size_t acceptedClientNum = 0;

  // continuously accept connections in while(1) loop  
  while(1) {  
    // accept any incoming connection  
    proxy_fd = accept(fd, (struct sockaddr*)NULL ,NULL);  
    cout << "accepted client (proxy server) with fd: " << proxy_fd << '\n';
    // if true then client request is accpted  
    if(proxy_fd > 0) {  
      acceptedClientNum = pool.getWorkingItemNumbers();
cout << "       acceptedClientNum=" << acceptedClientNum << '\n';
      if(acceptedClientNum >= thread_count) {
        cout << "Limited connection number has reached, you need to stop some previous connected channels." << '\n';
        //discard the accepted socket
        close(proxy_fd);
        continue;
      }

      //multithreading variables
      cout << "proxy connected" << '\n';
/***
                pthread_t tid;  
                // pass client fd as a thread parameter  
                pthread_create(&tid, NULL, runSocket, (void *)proxy_fd);   
***/
//      auto handle = std::async(launch::async, runSocket, proxy_fd);
      auto result = pool.enqueue(runSocket, proxy_fd);
    }  
  }  
  close(proxy_fd);   
  return 0;  
}
