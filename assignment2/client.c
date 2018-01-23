#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFFERSIZE 1100
struct client
{
    char id[7];
    char name[12];
    char tempbuf[BUFFERSIZE];
    int active;
};
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void* MapSharedMemory(int id)
{
	void* addr;
	addr = shmat(id, NULL, 0);
	return addr;
}
    int socket_c, portno, n;
    int clientshmid,clientshmkey,clientshmkey_sem,clientshmid_sem;
    struct client *clientptr;
    struct sockaddr_in server;
    struct hostent *serverh;
    char buffer[BUFFERSIZE];
    int pid;
    sem_t *client_semptr;
void* createshm(int *shmid,int key,int Size,int n,int flag)
{
		if(flag==0) *shmid=shmget(key,Size*n,IPC_CREAT | 0666);
		if(flag==1) *shmid=shmget(key,Size*n,0666);
		return MapSharedMemory(*shmid);
}
void destroyshm(int shmid)
{
	shmctl(shmid, IPC_RMID, NULL);
}
void deleteshm(void *shmptr)
{
	shmdt(shmptr);
}
int indexof(char * text,char toFind)
{
    int index = -1; // Assumes that the character doesn't exists
    int i, len;
    len = strlen(text);
    for(i=0; i<len; i++)
    {
        if(text[i] == toFind)
        {
            index = i;
            break;
        }
    }
    return index;
}
void parsing(char *tokens,char *buffer,int in)
{
        strncpy(tokens,buffer,in);
        tokens[in]='\0';
        strncpy(buffer,buffer+in+1,strlen(buffer)-in);
}
void close_handler(int signo)
{
    clientptr->active==0;
    if (signo == SIGINT)
    {
        kill(pid, SIGKILL);
        bzero(buffer,BUFFERSIZE);
        sprintf(buffer,"%s;%s;exit;NULL",clientptr->id,clientptr->name);
        if(write(socket_c,buffer,strlen(buffer))<0) error("\n ERROR:WRITE HELLO");
        bzero(buffer,BUFFERSIZE);
        if(read(socket_c,buffer,BUFFERSIZE-1)<0)error("\n ERROR:READ HELLO");
        printf("%s\n",buffer);
        close(socket_c);
        sleep(.4);
    }
    if(signo==0)
    {
    close(socket_c);
    sleep(.4);
    
    }
    kill(pid, SIGKILL);
    shmdt((void *)clientptr);
    destroyshm(clientshmid);
    exit(0);
}
int main(int argc, char *argv[])
{

    if (signal(SIGINT, close_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    portno = atoi(argv[2]);
    int t=(int)time(NULL);
    clientshmkey=t;
    clientshmkey_sem=t+1;
    clientptr=(struct client *)createshm(&clientshmid,clientshmkey,sizeof(struct client),1,0);
    client_semptr=(sem_t *)createshm(&clientshmid_sem,clientshmkey_sem,sizeof(sem_t),1,0);
    clientptr->active=1;
    socket_c = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_c < 0) error("\n ERROR opening socket");
    serverh = gethostbyname(argv[1]);
    if (serverh == NULL)
    {
        inet_aton(argv[1], &server);
        serverh = gethostbyaddr(&server, sizeof(server), AF_INET);
        if(serverh==NULL) error("\n ERROR, no such host\n");
    }
    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    bcopy((char *)serverh->h_addr,(char *)&server.sin_addr.s_addr,serverh->h_length);
    server.sin_port = htons(portno);
    if (connect(socket_c,(struct sockaddr *) &server,sizeof(server)) < 0) error("\n ERROR connecting");
    bzero(buffer,BUFFERSIZE);
    strcpy(buffer,"NULL;NULL;hello;NULL");
    if(write(socket_c,buffer,strlen(buffer))<0) error("\n ERROR:WRITE HELLO");
    pid=fork();
    if(pid>0)
    {
        clientptr=(struct client *)createshm(&clientshmid,clientshmkey,sizeof(struct client),1,1);
        client_semptr=(sem_t *)createshm(&clientshmid_sem,clientshmkey_sem,sizeof(sem_t),1,1);
        while(clientptr->active==1)
        {
            bzero(buffer,BUFFERSIZE);
            if(read(socket_c,buffer,BUFFERSIZE-1)<0)error("\n ERROR:READ HELLO");
            char tokens[5][400];
            bzero(tokens,1600);
            int i,j;
            for(j=0;j<3;j++)
            {
            i=indexof(buffer,';');
            parsing(tokens[j],buffer,i);
            }
            if(strcmp(tokens[2],"yourid")==0)
            {
                i=indexof(buffer,';');
                parsing(tokens[j],buffer,i);
                strcpy(clientptr->id,tokens[0]);
                strcpy(clientptr->name,tokens[1]);
                printf(" %s                      %s\n Your Details ID:%s    String:%s\n",tokens[3],buffer,tokens[0],tokens[1]);
            }
            if(strcmp(tokens[2],"exceed")==0)
            {
                printf("WARNING :%s\n",buffer);
                close_handler(0);
            }
            if(strcmp(tokens[2],"message")==0)
            {
                if(!(strcmp(tokens[0],"NULL")==0))
                    printf(" MSG By Id::%s, string::%s, msg::%s\n",tokens[0],tokens[1],buffer);
                else
                    printf(" Warining : %s\n",buffer);
            }
            if(strcmp(tokens[2],"activeid")==0)
            {
                    printf("\tUse either Id or String format as Id:message or String:message\nActive Clients");
                    char *token;
                    token = strtok(buffer,"#");
                    while( token != NULL )
                    {
                        printf( " %s\n", token );
                        token = strtok(NULL,"#");
                    }
            }
        }
    }
    if(pid==0)
    {
        sleep(.8);
        while(clientptr->active==1)
        {
            int choice=0;
            printf("Enter Choice \n\t1 for Online People\n\t2 For Send A Message \n");
            scanf("%d",&choice);
            if(choice==1)
            {
                sprintf(buffer,"%s;%s;getlist;%s",clientptr->id,clientptr->name,"NULL");
                if(write(socket_c,buffer,strlen(buffer))<0) error("\n ERROR:WRITE HELLO");
            }
            else if(choice==2)
            {
                char temp[BUFFERSIZE];
                char temp2[20];
                printf("Enter Id Or String:");
                scanf("%s",temp2);
                printf("Enter Message:");
                getchar();
                scanf("%[^\n]",temp);
                sprintf(buffer,"%s;%s;message;%s",clientptr->id,temp2,temp);
                if(write(socket_c,buffer,strlen(buffer))<0) error("\n ERROR:WRITE HELLO");
            }

        }
    }
    close(socket_c);
    return 0;
}
