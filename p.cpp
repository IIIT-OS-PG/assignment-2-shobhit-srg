#include<bits/stdc++.h>
#include <unistd.h> 
#include<iostream>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#define BUFF_SIZE 1024
using namespace std;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void * clientcopy(void *arg)
{
	int ret=1;
	int sockfd= *((int *)arg);
	char a[512];
	cout<<"ENTER FILE\n";
	cin>>a;
	cout<<a<<endl;
  	send(sockfd,&a,sizeof(a),0);
  	int file_size;
	recv(sockfd, &file_size, sizeof(file_size), 0);
	char y[512]="copy_";
	strcat(y,a);
	FILE *fp = fopen ( y, "wb" );
	char Buffer [ BUFF_SIZE] ; 
	

	int n;
	while ((n=recv(sockfd,Buffer,BUFF_SIZE,0))>0 && file_size >0)
	{
		fwrite (Buffer , sizeof (char), n, fp);
		memset ( Buffer , '\0', BUFF_SIZE);
		file_size = file_size - n;
	} 
	fclose(fp);
	close(sockfd);
	return NULL;
	//pthread_exit(&ret);

}
void * servercopy(void *arg)
{

	int ret=1;
	int sockfd= *((int *)arg);
	char a[512];
	recv(sockfd, &a, sizeof(a), 0);
	FILE *fp=fopen(a,"rb");
	fseek(fp,0,SEEK_END);
	int size=ftell(fp);
	rewind(fp);
	send(sockfd,&size,sizeof(size),0);
	char Buffer[BUFF_SIZE] ; 
	int n;
	while((n=fread(Buffer,sizeof(char),BUFF_SIZE,fp))>0 && size>0)
	{
			send(sockfd,Buffer,n,0);
	   	 	memset(Buffer,'\0',BUFF_SIZE);
			size=size-n;
	}
	cout<<"Sending file complete\n";
	fclose ( fp );
	close(sockfd);
	return NULL;
	//pthread_detach(pthread_self());
	//pthread_exit(&ret);

}

void  * server(void *yt)
{
	int server_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int sockfd;
	struct sockaddr_in   addr;
	int x;
	cout<<"ENTER RECEIVE PORT NO:";
	cin>>x;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(x);
	addr.sin_addr.s_addr=INADDR_ANY;


	cout<<"\nport no - "<<addr.sin_port<<"\n";

	int addrlen = sizeof(sockaddr);
	bind (server_fd  , (struct sockaddr *)&addr , sizeof ( addr ) );
	
	listen(server_fd,3);
	cout<<"LISTENING\n";
    // pthread_t tid[6];
    // int i = 0;
    while(1)
    {
    
    pthread_t t;
    addrlen=sizeof(addr);
	sockfd = accept ( server_fd , (struct sockaddr *)&addr,(socklen_t*)&addrlen);
    if( pthread_create(&t, NULL, &servercopy, &sockfd) != 0 )
    printf("Failed to create thread\n");
    }

	///close( sockfd);	
	close(server_fd);
	return NULL;
}

void * client(void *x)
{
	
	sleep(10);
    while(1)
	{
		int port;
		cout<<"CONNECT TO:";
		cin>>port;

	int sockfd = socket( AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in    serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

		if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
        {printf("\nConnection Failed \n");
        return NULL; }

        pthread_t t;
		if(pthread_create(&t,NULL, &clientcopy,&sockfd)!=0)
		printf("Failed\n");
		//pthread_join(t,NULL);
		//pthread_detach(pthread_self());
		// j++;
		close(sockfd);

		//sleep(5);
	}
	return NULL;
	
}



int main()
{
	
		
			pthread_t sid;
			if(pthread_create(&sid,NULL,&server,NULL)!=0)
				cout<<"failed";
	
			pthread_t cid;
			if(pthread_create(&cid,NULL,&client,NULL)!=0)
				cout<<"failed";

			pthread_join(cid,NULL);
}