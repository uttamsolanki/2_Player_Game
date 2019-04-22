#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>


int main(int argc, char *argv[])
{
  srand(time(NULL));
  signal(SIGINT, SIG_IGN);
  char message[100];
  int server, portNumber;
  socklen_t len;
  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
  }

 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }


  int clientP[2], serverP[2];
  clientP[0]=0;
  clientP[1]=0;
  serverP[0]=0;
  serverP[1]=0;
  char ch;

  while(1)
  {

    printf("\nPress Enter to play or Press '$' for exit\n");
    
    while(1)
    {
        
     //write operation//
      ch=fgetc(stdin);
      
      if(ch==0x0A)
      {
        clientP[0] = (rand() % 6)+1;
        clientP[1]+=clientP[0];
        break;
      }
      else if(ch=='$')
      {
        clientP[0]=0;
        clientP[1]=0;
        write(server, &clientP, sizeof(clientP));
        printf("\n\n I quit the game, Good bye server. \n");
        exit(7);        
      }
      else
      {
        clientP[0]=0;
        printf("Press enter to play");
      }
    }    

    printf("\nClient:-\n");
    printf("Got Points : %d\n", clientP[0]);
    printf("Total so far : %d\n", clientP[1]);
    printf("\n\n\n");
    write(server, &clientP, sizeof(clientP));
    //write operatin finished//
      
      
    //Read operation//
    if(clientP[1] >= 100)
    {
      if (read(server, &message, 50)<0){
        fprintf(stderr, "read() error\n");
        exit(3); 
      }
      
      printf("\n\nMessage From server :: %s \n",message);
      close(server);
      exit(5);
    }
    //Read opration finished//
      
      
    //Read operation//  
    if (read(server, &serverP, sizeof(serverP))<0){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }
    printf("\nServer:-\n");    
    printf("Got Points :%d \n",serverP[0]);
    printf("Total so far : %d\n",serverP[1]);
    //Read opration finished//
      
      
    //Read operation//  
    if(serverP[1] >= 100)
    {
      if (read(server, &message, 50)<0){
        fprintf(stderr, "read() error\n");
        exit(3); 
      }
      printf(" Message From server :: %s \n",message);
      close(server);
      exit(5);
    }
    //Read opration finished//


    //Read operation//  
    if (read(server, &message, 50)<0){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }

    printf("\n\n Message From server :: %s \n",message);
    //Read opration finished//
  }

  exit(0);
}
