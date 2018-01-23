#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define BUFFERSIZE 100

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void space_manager(char *buffer)
{
    int i=0,j=0;
    while(buffer[i]!='\0')
    {
        if(buffer[i]!=' ')buffer[j++]=buffer[i];
        i++;
    }
    buffer[j]='\0';
}
int main(int argc, char *argv[])
{
    int socket_c, portno, n;
    struct sockaddr_in server;
    struct hostent *serverh;
    char buffer[BUFFERSIZE];
    portno = atoi(argv[2]);
        socket_c = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_c < 0)
            error("ERROR opening socket");
        serverh = gethostbyname(argv[1]);
        if (serverh == NULL)
        {
            inet_aton(argv[1], &server);
            serverh = gethostbyaddr(&server, sizeof(server), AF_INET);
            if(serverh==NULL) error("ERROR, no such host\n");
        }
        bzero((char *) &server, sizeof(server));
        server.sin_family = AF_INET;
        bcopy((char *)serverh->h_addr,(char *)&server.sin_addr.s_addr,serverh->h_length);
        server.sin_port = htons(portno);
        if (connect(socket_c,(struct sockaddr *) &server,sizeof(server)) < 0)
            error("ERROR connecting");
        printf("Please enter the message: ");
        bzero(buffer,BUFFERSIZE);
        fgets(buffer,BUFFERSIZE,stdin);
        buffer[strlen(buffer)-1]='\0';
        char temp[BUFFERSIZE];
        strcpy(temp,buffer);
        space_manager(temp);
        int i = 0;
        while( temp[i] )
        {
            temp[i]=tolower(temp[i]);
            i++;
        }
        if(strcmp(temp,"exit")==0)
        {
            close(socket_c);
            exit(0);
        }
        else
        {
            n = write(socket_c,buffer,strlen(buffer));
            if(n < 0)
                error("ERROR writing to socket");
            bzero(buffer,BUFFERSIZE);
            n = read(socket_c,buffer,BUFFERSIZE-1);
            if (n < 0)
                error("ERROR reading from socket");
            printf("%s\n",buffer);
            close(socket_c);
        }
    return 0;
}
