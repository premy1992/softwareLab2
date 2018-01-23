#include<stdio.h>
#include<string>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<iostream>
#include <fstream>
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
#include <sys/time.h>
#include <queue>
#define BUFFERSIZE 1100
using namespace std;
class array_queue;
struct packet
{
    char data[BUFFERSIZE];
    char  time[25];
    char date[25];
    int pid;
    int active;
    int port;
};
struct client
{
    char id[7];
    char name[12];
    int pid;
    int active;
    int queue_index;
    char time[50];
};
int socket_s,socket_c;
int clientshmkey=1,clientshmid,clientshmkey_sem=2,clientshmid_sem;
int queueshmkey=3,queueshmid,queueshmkey_sem=4,queueshmid_sem;
//int child_queueshmkey[5]={9,11,13,15,17},child_queueshmid[5],child_queueshmkey_sem[5]={10,12,14,16,18},child_queueshmid_sem[5];
int child_queueshmkey=5,child_queueshmid,child_queueshmkey_sem=6,child_queueshmid_sem;
struct client *clientptr;
array_queue *queueptr,*child_queueptr;
sem_t *queue_semptr,*child_queue_semptr,*client_semptr;
int mpid,childs=5,*f;
int free_shmkey=7,free_shmid;
queue <int> cpid;
int bind_s;
int port=8888;
struct sockaddr_in server_address,client_address;

class array_queue
{
    private:
        int SIZE=200;
    public:
        int rear=-1,front=-1;
        struct packet ar[200];
        int enqueue(struct packet data);
        struct packet dequeue();
        struct packet getfront();
        struct packet getrear();
};
int array_queue::enqueue(struct packet value)
{
   if((front == 0 && rear == SIZE - 1) || (front == rear+1))
      return 0;
   else
   {
      if(rear == SIZE-1 && front != 0)  rear = -1;
      ar[++rear] = value;
      if(front == -1)   front = 0;
        return 1;
   }
}
struct packet array_queue::dequeue()
{
    if(front == -1 && rear == -1)
    {
        struct packet temp;
        memset(&temp,-1,sizeof(struct packet));
        return temp;
    }
    else
    {
        struct packet temp=ar[front++];
        if(front == SIZE)
        front = 0;
        {
        if(front-1 == rear)
        front = rear = -1;
        return temp;
        }
    }
}
struct packet array_queue::getrear()
{
    if(front == -1 && rear == -1)
    {
        struct packet temp;
        memset(&temp,-1,sizeof(struct packet));
        return temp;
    }
    else return ar[rear];
}
struct packet array_queue::getfront()
{
   if(front == -1 && rear == -1)
   {
        struct packet temp;
        memset(&temp,-1,sizeof(struct packet));
        return temp;
   }
   else return ar[front];
}
void* MapSharedMemory(int id)
{
	void* addr;
	addr = shmat(id, NULL, 0);
	return addr;
}
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
void generate_id(char temp[])
{
    sprintf(temp,"%d%d%d%d%d",
    rand()%10,rand()%10,rand()%10,rand()%10,rand()%10);
    temp[5]='\0';
}
void generate_name(char temp[])
{
    char str[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    sprintf(temp,"%c%c%c%c%c%c%c%c%c%c",
    str[rand()%63],str[rand()%63],str[rand()%63],str[rand()%63],str[rand()%63],
    str[rand()%63],str[rand()%63],str[rand()%63],str[rand()%63],str[rand()%63]);
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
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
void gettime(char *temp)
{
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(temp,"%d:%d:%d", tm.tm_hour, tm.tm_min, tm.tm_sec);
}
void getdate(char *temp)
{
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(temp,"%d-%d-%d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}
void close_handler(int signo)
{
    if (signo == SIGINT)
    {
        cout<<"shutdown..........."<<endl;
        while(cpid.size()>0) {kill(cpid.front(), SIGKILL);cpid.pop();}
        kill(mpid, SIGTERM);
        shmdt((void *)clientptr);
        destroyshm(clientshmid);

        shmdt((void *)queueptr);
        destroyshm(queueshmid);

        shmdt((void *)child_queueptr);
        destroyshm(child_queueshmid);

        sem_destroy(client_semptr);
        shmdt((void *)client_semptr);
        destroyshm(clientshmid_sem);

        sem_destroy(queue_semptr);
        shmdt((void *)queue_semptr);
        destroyshm(queueshmid_sem);

        sem_destroy(child_queue_semptr);
        shmdt((void *)child_queue_semptr);
        destroyshm(child_queueshmid_sem);

        close(socket_s);
        exit(1);
    }
}
void sendmsgbyid(char data[],char id[])
{
    int i;
    for(i=0;i<5;i++) if(strcpy(clientptr[i].id,id)==0) break;
    struct packet ob;
    strcpy(ob.data,data);
    child_queueptr[i].enqueue(ob);
    sem_post(child_queue_semptr+i);
}
void sendmsgbyindex(char data[],int i)
{
    struct packet ob;
    strcpy(ob.data,data);
    child_queueptr[i].enqueue(ob);
    sem_post(child_queue_semptr+i);
}
int getmyindex(int pid)
{
    int i;
    for(i=0;i<6;i++) {if(clientptr[i].pid==pid) break;}
    return i;
}
void sendmsgbypid(struct packet ob)
{
    int i=getmyindex(ob.pid);
    child_queueptr[i].enqueue(ob);
    sem_post(child_queue_semptr+i);
}
void monitor()
{
    cout<<"Monitor Process Active:"<<endl;
	char buffer[BUFFERSIZE];
    clientptr=(struct client *)createshm(&clientshmid,clientshmkey,sizeof(struct client),6,1);
    client_semptr=(sem_t *)createshm(&clientshmid_sem,clientshmkey_sem,sizeof(sem_t),1,1);

    queueptr=(array_queue *)createshm(&queueshmid,queueshmkey,sizeof(array_queue),1,1);
    queue_semptr=(sem_t *)createshm(&queueshmid_sem,queueshmkey_sem,sizeof(sem_t),1,1);

    child_queueptr=(array_queue *)createshm(&child_queueshmid,child_queueshmkey,sizeof(array_queue),6,1);
    child_queue_semptr=(sem_t *)createshm(&child_queueshmid_sem,child_queueshmkey_sem,sizeof(sem_t),6,1);

    f=(int*)createshm(&free_shmid,free_shmkey,sizeof(int),1,1);

    while(1)
    {
        sem_wait(queue_semptr);
        struct packet ob=queueptr->dequeue();
        if(strcmp(ob.data,"")==0||ob.port==-1)continue;
        char temp[BUFFERSIZE];
        char tokens[5][100];
        bzero(tokens,500);
        cout<<"data dequeued  "<<ob.data<<"     f =: "<<*f <<endl;
        for(int j=0;j<3;j++)
            {parsing(tokens[j],ob.data,indexof(ob.data,';'));}
        if(strcmp(tokens[2],"hello")==0)
        {
            int active_count=0;
            char id[6];
            char name[11];
            generate_id(id);
            generate_name(name);
            for(int i=0;i<6;i++) if(clientptr[i].active==1)active_count++;
            if(active_count<=5)
            {
                strcpy(clientptr[*f].name,name);
                strcpy(clientptr[*f].id,id);
                sprintf(clientptr[*f].time,"Time:%s Date:%s",ob.time,ob.date);
                sprintf(ob.data,"%s;%s;yourid;Connection Established;%s",id,name,clientptr[*f].time);
                for(int j=0;j<6;j++) if(clientptr[j].active==0) {*f=j;break;}
                cout<<"f  "<<*f<<endl;
                sendmsgbypid(ob);
            }
            else
            {
                strcpy(ob.data,"NULL;NULL;exceed;Connection Limit Exceed");
                ob.active=1;
                sendmsgbypid(ob);
            }
        }
        if(strcmp(tokens[2],"getlist")==0)
        {
            int i;
            strcpy(temp,"\n  ID    String  ");
            for(i=0;i<6;i++)
            {
                if(clientptr[i].active==1&&!(strcmp(tokens[0],clientptr[i].id)==0))
                {
                    sprintf(temp,"%s#%s   %s",temp,clientptr[i].id,clientptr[i].name);
                }
            }
            sprintf(ob.data,"%s;%s;activeid;%s",tokens[0],tokens[1],temp);
            sendmsgbypid(ob);
        }
        if(strcmp(tokens[2],"message")==0)
        {

            int i,j;
            for(i=0;i<6;i++) if(clientptr[i].active==1&&strcmp(tokens[0],clientptr[i].id)==0) break;
            strcpy(temp,ob.data);
            for(j=0;j<6;j++) if(clientptr[j].active==1&&!strcmp(tokens[0],clientptr[j].id)==0&&
            (strcmp(tokens[1],clientptr[j].id)==0||strcmp(tokens[1],clientptr[j].name)==0)) break;
            if(j<6)
            {   cout<<"message  "<<*f<<":active i:j "<<i<<j<<endl;
                ofstream of ("log.csv",ios::out | ios::in | ios::app);
                of<<ob.date<<","<<ob.time<<","<<clientptr[i].id<<","<<clientptr[i].name<<","<<clientptr[j].id<<","<<clientptr[j].name<<","<<ob.data<<"\n";
                of.close();
                sprintf(ob.data,"%s;%s;message;%s",tokens[0],clientptr[i].name,temp);
                ob.pid=clientptr[j].pid;
            }
            else
            {
                sprintf(ob.data,"NULL;NULL;message;Invalid Details");
            }
            sendmsgbypid(ob);

        }
        if(strcmp(tokens[2],"exit")==0)
        {
            int i;
            for(i=0;i<6;i++) if(clientptr[i].active==1&&!(strcmp(tokens[0],clientptr[i].id)==0))
            {
                struct packet ob1;
                sprintf(ob1.data,"%s;%s;message;%s",tokens[0],tokens[1],"I am going to hell");
                ob1.pid=clientptr[i].pid;
                sendmsgbypid(ob1);
            }
            ob.active=1;
            strcpy(ob.data,"Connection Closed : shutdown");
            sendmsgbypid(ob);
            sleep(.1);
        }
    }
}
void child()
{
    close(socket_s);
    clientptr=(struct client *)createshm(&clientshmid,clientshmkey,sizeof(struct client),6,1);
    client_semptr=(sem_t *)createshm(&clientshmid_sem,clientshmkey_sem,sizeof(sem_t),1,1);

    queueptr=(array_queue *)createshm(&queueshmid,queueshmkey,sizeof(array_queue),1,1);
    queue_semptr=(sem_t *)createshm(&queueshmid_sem,queueshmkey_sem,sizeof(sem_t),1,1);

    child_queueptr=(array_queue *)createshm(&child_queueshmid,child_queueshmkey,sizeof(array_queue),6,1);
    child_queue_semptr=(sem_t *)createshm(&child_queueshmid_sem,child_queueshmkey_sem,sizeof(sem_t),6,1);
    f=(int*)createshm(&free_shmid,free_shmkey,sizeof(int),1,1);
    int in=*f;
    int pid=getpid();
    clientptr[in].queue_index=in;
    clientptr[in].active=1;
    clientptr[in].pid=pid;
    int cpid=fork();
    ///////////////sending////////////
    if(cpid>0)
    {
        clientptr=(struct client *)createshm(&clientshmid,clientshmkey,sizeof(struct client),6,1);
        client_semptr=(sem_t *)createshm(&clientshmid_sem,clientshmkey_sem,sizeof(sem_t),1,1);

        queueptr=(array_queue *)createshm(&queueshmid,queueshmkey,sizeof(array_queue),1,1);
        queue_semptr=(sem_t *)createshm(&queueshmid_sem,queueshmkey_sem,sizeof(sem_t),1,1);

        child_queueptr=(array_queue *)createshm(&child_queueshmid,child_queueshmkey,sizeof(array_queue),6,1);
        child_queue_semptr=(sem_t *)createshm(&child_queueshmid_sem,child_queueshmkey_sem,sizeof(sem_t),6,1);

        f=(int*)createshm(&free_shmid,free_shmkey,sizeof(int),1,1);

        while(clientptr[in].active==1)
        {   cout<<"sender in "<<in<<"  f "<<*f<<endl;
            sem_wait(child_queue_semptr+in);
            struct packet ob=child_queueptr[in].dequeue();
            if(write(socket_c,ob.data,strlen(ob.data))<0) {perror("ERROR:YourId write Error");break;}
            if(ob.active==1) {clientptr[in].active=0; break;}
        }
        for(int j=0;j<6;j++) if(clientptr[j].active==0) {*f=j;break;}
       // close(socket_c);
        /*shmdt((void *)clientptr);
        shmdt((void *)queueptr);
        shmdt((void *)child_queueptr);
        sem_destroy(client_semptr);
        shmdt((void *)client_semptr);
        sem_destroy(queue_semptr);
        shmdt((void *)queue_semptr);
        sem_destroy(child_queue_semptr);
        shmdt((void *)child_queue_semptr);*/
        kill(cpid,SIGKILL);
    }
    //////////////recieving//////////
    if(cpid==0)
    {
        while(clientptr[in].active==1)
        {   struct packet ob;
            memset((char *)&ob.data,0,BUFFERSIZE);
            int n=read(socket_c,ob.data,BUFFERSIZE);
            if(n<0)
            {
                perror("ERROR: READ");
                break;
            }
            if(strcmp(ob.data,"")==0){break;}
            getdate(ob.date);
            gettime(ob.time);
            ob.pid=pid;
            int client_port =(int) ntohs(client_address.sin_port);
            ob.port=client_port;
            queueptr->enqueue(ob);
            cout<<"reciever in "<<in<<"   f "<<*f<<endl;
            sem_post(queue_semptr);
        }
        /*shmdt((void *)clientptr);
        shmdt((void *)queueptr);
        shmdt((void *)child_queueptr);
        sem_destroy(client_semptr);
        shmdt((void *)client_semptr);
        sem_destroy(queue_semptr);
        shmdt((void *)queue_semptr);
        sem_destroy(child_queue_semptr);*/
    }
}
int main(int argc,char *argv[])
{
    if (signal(SIGINT, close_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
//////////////////////////////SHARED MEMORY///////////////////////////////////////
    clientptr=(struct client *)createshm(&clientshmid,clientshmkey,sizeof(struct client),6,0);
    client_semptr=(sem_t *)createshm(&clientshmid_sem,clientshmkey_sem,sizeof(sem_t),1,0);

    queueptr=(array_queue *)createshm(&queueshmid,queueshmkey,sizeof(array_queue),1,0);
    queue_semptr=(sem_t *)createshm(&queueshmid_sem,queueshmkey_sem,sizeof(sem_t),1,0);

    child_queueptr=(array_queue *)createshm(&child_queueshmid,child_queueshmkey,sizeof(array_queue),6,0);
    child_queue_semptr=(sem_t *)createshm(&child_queueshmid_sem,child_queueshmkey_sem,sizeof(sem_t),6,0);

    f=(int*)createshm(&free_shmid,free_shmkey,sizeof(int),1,0);
    *f=0;

    memset((char *)clientptr,0,sizeof(struct client)*6);
    memset((char *)queueptr,0,sizeof(array_queue));
    memset((char *)child_queueptr,0,sizeof(array_queue)*6);
    if (sem_init(client_semptr, 1, 0) == -1)cout<<"sem_init: failed: client_semptr "<<endl;
    if (sem_init(queue_semptr, 1, 0) == -1)cout<<"sem_init: failed: queue_semptr "<<endl;
    for(int j=0;j<6;j++) if (sem_init(child_queue_semptr+j, 1, 0) == -1) cout<<"sem_init: failed: client_queue_semptr "<<endl;
    for(int i=0;i<6;i++)
    child_queueptr[i].dequeue();
    queueptr->dequeue();
////////////////////////////////FORK process//////////////////////////////
    if((mpid=fork())==0)
    {
        monitor();
        exit(0);
    }
    //port=atoi(argv[1]);
    int opt = 1;
    if((socket_s=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("ERROR: socket discrepter not generated");
        close_handler(SIGINT);
	    exit(-1);
    }
    if( setsockopt(socket_s, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        close_handler(SIGINT);
        exit(EXIT_FAILURE);
    }
    server_address.sin_family =AF_INET;
    server_address.sin_port=htons(port);;
    server_address.sin_addr.s_addr=INADDR_ANY;
    bzero(&server_address.sin_zero,8);
    if((bind_s=bind(socket_s,(struct sockaddr *)&server_address,sizeof(server_address)))<0)
    {
	perror("ERROR: BIND");
	close_handler(SIGINT);
	exit(-1);
    }
    if(listen(socket_s,5)<0)
    {
	perror("ERROR: LIsten");
	close_handler(SIGINT);
	exit(-1);
    }
    ofstream of ("log.csv",ios::out | ios::in | ios::trunc);
    of<<"Date,Time,Sender Id,Sender String,Reciver Id,Reciever String,Message\n";
    of.close();
    printf("Waiting For Request \n");
    while(1)
    {
        int len=sizeof(client_address);
        if((socket_c=accept(socket_s,(struct sockaddr *)&client_address,(socklen_t *)&len))<0)
    	{
	    perror("ERROR: ACCEPT");
	    close_handler(SIGINT);
	    exit(-1);
    	}
        int p=fork();
        if(p==0)
        {
            child();
            exit(0);
        }
        cpid.push(p);
    }
    return 0;
}
