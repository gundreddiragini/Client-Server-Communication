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

/*Main Function*/
int main(int argc,char *argv[])
{
if(argc>1)
{
}

/*Variable declaration*/
int sockfd = 0;               // for storing the socket id of the socket that is created
struct sockaddr_in serv_addr;    // structure for storing the details of the port to bind a socket


/* Create a socket first */
if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
{
printf("\n Error : Could not create socket \n");
return 1;
}


/* Initialize sockaddr_in data structure */
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(atoi(argv[1])); // port
serv_addr.sin_addr.s_addr = inet_addr(argv[2]);


/* Attempt a connection */
if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
{
printf("\n Error : Connect Failed \n");
return 1;
}
else
printf("Server accepted the connection\n");

/*Passing the output file name to the server*/
write(sockfd, argv[4], sizeof(argv[4]));

/* Open the file that we wish to transfer */
char *bu=argv[3]; 
FILE *fp = fopen(bu,"rb");
if(fp==NULL)
{
printf("File opern error");
return 1;   
}   

/* Read data from file and send it */
while(1)
{
/* First read file in chunks of 10 bytes */
unsigned char buff[10]={0};
int nread = fread(buff,1,10,fp);
            
if(nread>4)
printf(" %d bytes are read from the file \n", nread);        //Printing the number of bytes that are read from the file


/* If read was success, send data. */
if(nread > 4)
{
printf("Sending the data that is read\n");
write(sockfd, buff, nread);
}


/*
* There is something tricky going on with read .. 
* Either there was error, or we reached end of file.
*/
if (nread < 2)
{
if (feof(fp))
printf("End of file\n");
if (ferror(fp))
printf("Error reading\n");
break;
}

}

return 0;
}


