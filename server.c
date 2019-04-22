#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <time.h>

static int client_count = 0;

void serviceClient(int client, int client_number)
{
  int clientP[2], serverP[2];
  clientP[0]=0;
  clientP[1]=0;
  serverP[0]=0;
  serverP[1]=0;
  char message[255];

  while(1)
  {
    
    if(read(client, &clientP, sizeof(clientP))<0)
    {
      fprintf(stderr, "read() error\n");
      exit(3);
    }
    if(clientP[0] == 0)
    {
      printf("\n\n Client %d has left. \n\n",client_number);
      close(client);
      exit(5);      
    }
    printf("\n**********************************\n");
    printf("Client:-\n");
    printf("Got Points : %d\n",clientP[0]);
    printf("Total so far : %d\n\n\n",clientP[1]);
    

    if(clientP[1] >= 100)
    {
      write(client,"Game over: You won the game.",50);
      printf("Client won!!!\n");
      printf("\n\n Client %d is gone. \n\n",client_number);  
      close(client);
      exit(5);
    }
   
    serverP[0] = (rand() % 6)+1;
    serverP[1]+=serverP[0];
    printf("Server:-\n");
    printf("Got Points : %d\n", serverP[0]);
    printf("Total so far : %d\n", serverP[1]);
    write(client, &serverP, sizeof(serverP));
      
    if(serverP[1] >= 100)
    {
      write(client,"Game over: You lost the game.",50);      


      printf("\n\n Client %d is gone. \n\n",client_number);
        
      close(client);
      exit(5);
    }
    //Write operation finished//
      
      
    //Write operation//
    write(client,"Game on: you can now play your dice",50);
    //Write operation finished//      
    printf("\n\n");
  }
    exit(1); 
}

int main(int argc, char *argv[]){  

  srand(time(NULL));
  pid_t p;
  int n;
  int sd, client, portNumber;
  socklen_t len;
  char *currentTime;
  time_t currentUnixTime;

  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    client = accept(sd, NULL, NULL);
    client_count++;
    printf("\n Client %d joined \n",client_count);
    if(!fork())
    {
      serviceClient(client, client_count);
    }    
    // wait(&n);
  }
    close(client);  
}
