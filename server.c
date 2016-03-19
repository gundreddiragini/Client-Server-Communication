#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include<time.h>

/*Main Function*/
int main(void)
{

/*Variable declaration*/
int listenfd = 0;                 // for storing the socket id of the socket that is created
int connfd = 0;                   //for storing the id of incoming connections  
struct sockaddr_in serv_addr;     // structure for storing the details of the port to bind a socket
int bytesReceived = 0;            //For storing the number of bytes recieved from the client
char recvBuff[10],tempBuff[5];    //Buffers that are used to store the data recieved from the client 
memset(recvBuff, '0', sizeof(recvBuff));


/* Create a socket first */
listenfd = socket(AF_INET, SOCK_STREAM, 0);
printf("Socket retrieve success\n");
memset(&serv_addr, '0', sizeof(serv_addr));


/* Initialize sockaddr_in data structure */
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
serv_addr.sin_port = htons(5000);


/*Binding the socket to the port*/
bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
if(listen(listenfd, 10) == -1)
{
printf("Failed to listen\n");
return -1;
}
printf("Waiting for incoming connections from clients\n");


while(1)
{
/*For accepting the connections that are obtained form clients*/
connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
printf("A connection request from a client is accepted\n");

/*Read data that is written on the socket by the client (to know the output file name)*/
bytesReceived = read(connfd, recvBuff, 20);

/* Create file where data will be stored */
FILE *fp;
int o=1;
if (o==1)
fp = fopen(recvBuff, "ab");
else
fp = fopen("image.jpg", "ab");
if(NULL == fp)
{
printf("Error opening file");
return 1;
}   


else
{
printf("File is created successfully \n");
}      

/* Receive data in chunks of 10 bytes */
while((bytesReceived = read(connfd, recvBuff, 10)) > 0)
{
int i;           //temporary variable

/*Store the 10 byte chunks recieved as 5 byte chunks so that only 5 bytes can be written at a time into the file*/
for(i=0;i<=4;i++)
tempBuff[i]=recvBuff[i];
for(i=0;i<=4;i++)
recvBuff[i]=recvBuff[i+5];

/*Printing on the terminal*/
printf("Bytes received %d\n",bytesReceived); 
printf("Copying first 5 bytes into the file initially\n"); 
printf("Data that is being copied into the file is:");
for(i=0;i<=4;i++)
printf("%c ",tempBuff[i]);
printf("\n");   
fprintf(fp, "%c%c%c%c%c", tempBuff[0], tempBuff[1], tempBuff[2], tempBuff[3], tempBuff[4]);  //Copying chunk of 1st 5 bytes into the output file


printf("Copying the remaining 5 bytes into the file\n");
for(i=0;i<=4;i++)
tempBuff[i]=recvBuff[i];
printf("Data that is being copied into the file is:");
for(i=0;i<=4;i++)
printf("%c ",tempBuff[i]);
printf("\n");   
fprintf(fp, "%c%c%c%c%c", tempBuff[0], tempBuff[1], tempBuff[2], tempBuff[3], tempBuff[4]);  //Copying chunk of remaining 5 bytes into the output file 
}

/*situation where nothing is recieved from the client */
if(bytesReceived < 0)
{
printf("\n Read Error \n");
}

close(connfd);

/*Waiting for some other clients*/
printf("Waiting for other clients for two seconds...\n");
int t;                           // for defining a timer 
time_t start,end;                // for defining a timer
start=clock();                   //predefined  function in c

do
{
end=clock();
t=(end-start)/CLOCKS_PER_SEC;
}while(t<2);
printf("Completed waiting for two seconds....\n");
printf("No other clients are currently available....\n");
break;

}

return 0;
}
