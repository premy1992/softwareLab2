#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<signal.h>
#define BUFFERSIZE 100
int socket_s;
struct stat
{
int counter;
int index;
};
struct stat occurencecounter_naive(char text[],char pattern[])
{
    int i=0;
    int lp=strlen(pattern),lt=strlen(text);
    struct stat ob;
    ob.counter=0;
    ob.index=-1;
    while(i<lt-lp+1)
    {
        int j=0;
        while(text[i+j]==pattern[j]&&j<lp){j++;}
        if(j==lp) {ob.index=i-j+1;ob.counter++;}
        i++;
    }
    return ob;
}
// Fills lps[] for given patttern pat[0..M-1]
void pattern_preprocessing(char *pat, int M, int *lps)
{
    // length of the previous longest prefix suffix
    int j = 0;
    lps[0] = 0; // lps[0] is always 0
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[j])
        {
            j++;
            lps[i] = j;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (j != 0)
            {
                j = lps[j-1];
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
                lps[i] = 0;
        }
        i++;
    }
}
struct stat occurencecounter_kmp(char text[],char pattern[])
{
    int M = strlen(pattern);
    int N = strlen(text);
    struct stat ob;
    ob.counter=0;
    ob.index=-1;
    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int lps[M];

    // Preprocess the pattern (calculate lps[] array)
    pattern_preprocessing(pattern, M, lps);
    int i = 0;  // index for txt[]
    int j  = 0;  // index for pat[]
    while (i < N)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == M)
        {
            ob.counter++;
            ob.index=i-j;
            j = lps[j-1];
        }

        // mismatch after j matches
        else if (i < N && pattern[j] != text[i])
        {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j-1];
            else
                i = i+1;
        }
    }
    return ob;
}
struct stat occurencecounter_keyword(char line[],char pattern[])
{
    int M = strlen(pattern);
    int N = strlen(line);
    struct stat ob;
    ob.counter=0;
    ob.index=-1;
    char token[20][20];
    int i=0,l=0,index=0;
    while(1)
    {
        if(line[i]=='\n'||line[i]=='\0')
        {
            strncpy(token[l],line+index,i-index);
            token[i-1][strlen(token[i-1])-1]='\0';
            l++;
            break;
        }
        else if(line[i]==' ')
        {
            strncpy(token[l],line+index,i-index);
            l++;
            index=i+1;
        }
			i++;
    }
    for (i = 0; i< l; i++)
    {
        if(strcmp(token[i],pattern)==0) ob.counter++;
    }
    return ob;
}
void parsing(char buffer[],char text[],char pattern[],char delimeter[])
{
    struct stat ob=occurencecounter_kmp(buffer,delimeter);
    if(ob.counter==1)
    {
        strncpy(text,buffer,ob.index);
        text[ob.index]='\0';
        strncpy(pattern,buffer+ob.index+1,strlen(buffer)-ob.index);
    }
    else
    {
        strcpy(text,"");
        strcpy(pattern,"");
    }
}
void space_manager1(char *buffer)
{
    int i=0,j=0;
    int flag=0;
    while(buffer[i]!='\0')
    {
        if(buffer[i]==' '&&flag==0){flag==1;j++;}
        if(buffer[i]==' '&&flag==1){buffer[j++]=buffer[i];}
        if(buffer[i]!=' '){flag=0;buffer[j++]=buffer[i];}
        i++;
    }
    buffer[j]='\0';
}
void space_manager2(char *buffer)
{
    int i=0,j=0;
    while(buffer[i]!='\0')
    {
        if(buffer[i]!=' ')buffer[j++]=buffer[i];
        i++;
    }
    buffer[j]='\0';
}
int illegal_tester(char *buffer)
{
    int flag=0,i=0;
    while(buffer[i]!='\0')
    {
        if(!(buffer[i]==' '||buffer[i]>=65&&buffer[i]<=90||buffer[i]>=97&&buffer[i]<=122)) flag=1;
        i++;
    }
    return flag;
}
void close_handler(int signo)
{
    if (signo == SIGINT)
    {
        close(socket_s);
        exit(1);
    }
}
void main(int argc,char *argv[])
{
    char text[BUFFERSIZE],pattern[50],delimeter[10]=";";
    int socket_c,bind_s;
    int port=8888;
    char buffer[BUFFERSIZE];
    int send_size,childpid;
    struct sockaddr_in server_address,client_address;
    if (signal(SIGINT, close_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if((socket_s=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
	perror("ERROR: socket discrepter not generated");
	exit(-1);
    }
    server_address.sin_family =AF_INET;
    server_address.sin_port=htons(port);;
    server_address.sin_addr.s_addr=INADDR_ANY;
    bzero(&server_address.sin_zero,8);
    if((bind_s=bind(socket_s,(struct sockaddr *)&server_address,sizeof(server_address)))<0)
    {
	perror("ERROR: BIND");
	exit(-1);
    }
    if(listen(socket_s,10)<0)
    {
	perror("ERROR: LIsten");
	exit(-1);
    }
    printf("Waiting For Request \n");
    int len=sizeof(struct sockaddr_in);
    while(1)
    {
        if((socket_c=accept(socket_s,(struct sockaddr *)&client_address,&len))<0)
    	{
	    perror("ERROR: ACCEPT");
	    exit(-1);
    	}
        char *client_ip =(char *) inet_ntoa(client_address.sin_addr);
        int client_port =(int *) ntohs(client_address.sin_port);
        printf(" client  details %s:%d \n",client_ip,client_port);
        memset(buffer,0,BUFFERSIZE);
        if ((childpid = fork()) == 0)
        {

            close(socket_s);
            char message[100];
            char tempp[31],tempt[31],tempb[31];
            int n=read(socket_c,buffer,BUFFERSIZE);
            if(n<0){perror("ERROR: In Reading");exit(-1);}
            printf("\trecieved data:  %s\n",buffer,n);
            buffer[n]='\0';
            parsing(buffer,text,pattern,delimeter);
            strcpy(tempb,buffer);
            strcpy(tempt,text);
            strcpy(tempp,pattern);
            int p=illegal_tester(pattern);
            int t=illegal_tester(text);
            space_manager2(tempb);
            space_manager2(tempp);
            space_manager2(tempt);
            int lenp=strlen(pattern);
            int lent=strlen(text);
            int lenb=strlen(tempb);
            strcpy(message,"text:: ");
            strcat(message,text);
            strcat(message,"\nkey::  ");
            strcat(message,pattern);
            printf("buffer:%s:  text:%s:  pattern:%s:\n",buffer,text,pattern);
            if(lenb==0)  {strcpy(message,"Nothing send by you");}
            else if(p==0&&t==0&&lenp==0&&lent==0)  {strcat(message,"\nwarning ::  ");strcat(message,"Both text and pattern empty");}
            else if(p==0&&t==0&&lenp==0&&lent!=0&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern is empty");}
            else if(p==0&&t==0&&lenp==0&&lent!=0&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern is empty and text is out of limit");}
            else if(p==0&&t==0&&lenp!=0&&lent==0&&lenp<=5)  {strcat(message,"\nwarning ::  ");strcat(message,"text is empty");}
            else if(p==0&&t==0&&lenp!=0&&lent==0&&lenp>5)  {strcat(message,"\nwarning ::  ");strcat(message,"text is empty  and pattern is out of limit");}
            else if(p==0&&t==0&&lenp!=0&&lent!=0&&lenp<=5&&lent<=30)
                {
                    char str[10];
                    struct stat ob=occurencecounter_keyword(text,pattern);
                    sprintf(str, "%d",ob.counter);
                    strcat(message,"\noccurence count::  ");
                    strcat(message,str);
                }
            else if(p==0&&t==0&&lenp!=0&&lent!=0&&lenp<=5&&lent>30) {strcat(message,"\nwarning ::  ");strcat(message,"text is out of limit");}
            else if(p==0&&t==0&&lenp!=0&&lent!=0&&lenp>5&&lent<=30) {strcat(message,"\nwarning ::  ");strcat(message,"pattern is out of limit");}
            else if(p==0&&t==0&&lenp!=0&&lent!=0&&lenp>5&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"both pattern and text is out of limit");}
            else if(p==0&&t!=0&&lenp==0&&lent==0)  {strcat(message,"\nwarning ::  ");strcat(message,"Both text and pattern empty,text contain illegal character");}
            else if(p==0&&t!=0&&lenp==0&&lent!=0&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern is empty and text contain illegal character");}
            else if(p==0&&t!=0&&lenp==0&&lent!=0&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern is empty and text contain illegal character and text exceed the limit");}
            else if(p==0&&t!=0&&lenp!=0&&lent==0&&lenp<=5)  {strcat(message,"\nwarning ::  ");strcat(message,"text is empty and text contain illegal character");}
            else if(p==0&&t!=0&&lenp!=0&&lent==0>5)  {strcat(message,"\nwarning ::  ");strcat(message,"text is empty and contain illegal character and pattern exceed the limit");}
            else if(p==0&&t!=0&&lenp!=0&&lent!=0&&lenp<=5&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"text contain illegal character");}
            else if(p==0&&t!=0&&lenp!=0&&lent!=0&&lenp<=5&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"text contain illegal character and exceed the limit");}
            else if(p==0&&t!=0&&lenp!=0&&lent!=0&&lenp>5&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"text contain illegal character and pattern exceed the limit");}
            else if(p==0&&t!=0&&lenp!=0&&lent!=0&&lenp>5&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"text contain illegal character and both pattern and text exceed the limit");}

            else if(p!=0&&t==0&&lenp==0&&lent==0)  {strcat(message,"\nwarning ::  ");strcat(message,"Both text and pattern empty and pattern contain illegal character");}
            else if(p!=0&&t==0&&lenp==0&&lent!=0&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern is empty and contain illegal character");}
            else if(p!=0&&t==0&&lenp==0&&lent!=0&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern is empty and contain illegal character and text exceed the limit");}
            else if(p!=0&&t==0&&lenp!=0&&lent==0&&lenp<=5)  {strcat(message,"\nwarning ::  ");strcat(message,"text is empty and pattern contain illegal character");}
            else if(p!=0&&t==0&&lenp!=0&&lent==0&&lenp>5)  {strcat(message,"\nwarning ::  ");strcat(message,"text is empty and pattern contain illegal character and its exceed the limit");}
            else if(p!=0&&t==0&&lenp!=0&&lent!=0&&lenp<=5&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern contain a illegal character");}
            else if(p!=0&&t==0&&lenp!=0&&lent!=0&&lenp<=5&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern contain a illegal character and text is out of limit");}
            else if(p!=0&&t==0&&lenp!=0&&lent!=0&&lenp>5&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern contain a illegal character and out of limit");}
            else if(p!=0&&t==0&&lenp!=0&&lent!=0&&lenp>5&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern contain a illegal character, both pattern and text  are out of limit");}

            else if(p!=0&&t!=0&&lenp==0&&lent==0)  {strcat(message,"\nwarning ::  ");strcat(message,"Both text and pattern empty,both text and pattern contain illegal character");}
            else if(p!=0&&t!=0&&lenp==0&&lent!=0&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern is empty, both pattern and text contain illegal character");}
            else if(p!=0&&t!=0&&lenp==0&&lent!=0&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"pattern is empty, both pattern and text contain illegal character and text is out of limit");}
            else if(p!=0&&t!=0&&lenp!=0&&lent==0&&lenp<=5)  {strcat(message,"\nwarning ::  ");strcat(message,"text is empty, both pattern and text contain illegal character ");}
            else if(p!=0&&t!=0&&lenp!=0&&lent==0&&lenp>5)  {strcat(message,"\nwarning ::  ");strcat(message,"text is empty, both pattern and text contain illegal character and pattern is out of limit");}
            else if(p!=0&&t!=0&&lenp!=0&&lent!=0&&lenp<=5&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"both pattern and text contain illegal character");}
            else if(p!=0&&t!=0&&lenp!=0&&lent!=0&&lenp<=5&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"both pattern and text contain illegal character and text is out of limit");}
            else if(p!=0&&t!=0&&lenp!=0&&lent!=0&&lenp>5&&lent<=30)  {strcat(message,"\nwarning ::  ");strcat(message,"both pattern and text contain illegal character and pattern is out of limit");}
            else if(p!=0&&t!=0&&lenp!=0&&lent!=0&&lenp>5&&lent>30)  {strcat(message,"\nwarning ::  ");strcat(message,"both pattern and text contain illegal character and both text and pattern are out of limit");}
            n=write(socket_c,message,strlen(message));
            if(n<0){perror("ERROR: In Writing");exit(-1);}
            close(socket_c);
            exit(0);
        }
        close(socket_c);
    }
    close(socket_s);
}
