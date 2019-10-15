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
int serverport;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int port1[512];
// struct info{
// 	int sock;
// 	char* name;
// };
int* portrequest(char* a1,int option)
{
		cout<<"filename:::  "<<a1<<endl;
		char a[512];
		strcpy(a,a1);
		cout<<"a\t"<<a<<endl;
		
	 	int sockfd = socket( AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in    serv_addr;
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(8888);
		serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

		if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
        {printf("\nConnection Failed \n");
        }
       
		send(sockfd,&option,sizeof(option),0);
        send(sockfd,&a,sizeof(a),0);
        vector<int> port;
        int sz;
        recv(sockfd, &sz, sizeof(sz), 0);
        //cout<<sz<<"    \n";
        // recv(sockfd, &port, sizeof(port), 0);
        // int pp,pp1;
        // recv(sockfd, &pp, sizeof(pp), 0);
        // recv(sockfd, &pp1, sizeof(pp1), 0);
        // cout<<pp<<pp1;
        // auto j=0;
        int j=0;
        int porr[512];
        for(int i=0;i<sz;i++)
        {
        	recv(sockfd, &porr[i], sizeof(porr[i]), 0);
        	port1[j]=porr[i];
        	//cout<<porr[i]<<" ";
        	//port[j]=porr[i];
        	j++;
        }
        port1[j]=sz;
        cout<<port1[j]<<"\n";
       // cout<<endl;
    //     for(auto i=0;i<sz;i++)
				// {
			
				// 	cout<<"dsgsdh";
				// 	recv(sockfd, &pp, sizeof(pp), 0);
				// 	port[i]=pp;
				// 	cout<<port[i]<<" ";
				// }
				// cout<<endl;
        close(sockfd);
        return port1;
}

void * clientcopy(void *arg)
{
		cout<<"0 for upload & 1 for download\n";
		int option;
		cin>>option;
		if(option==0)
		{
				int sockfd = socket( AF_INET, SOCK_STREAM, 0);
				struct sockaddr_in    serv_addr;
				serv_addr.sin_family = AF_INET;
				serv_addr.sin_port = htons(8888);
				serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
		
				if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
		        {printf("\nConnection Failed \n");
		        return NULL; }

		        char a[512];
				cout<<"ENTER FILE to upload\n";
				cin>>a;
				send(sockfd,&option,sizeof(option),0);
				send(sockfd,&a,sizeof(a),0);
				send(sockfd,&serverport,sizeof(serverport),0);
				close(sockfd);
		}
		if(option==1)
		{
				
				cout<<"CONNECTED TO TRACKER :8888\n";
				char a[512];
				cout<<"ENTER FILE\n";
				cin>>a;
				//vector<int>port;
				int* porr=portrequest(a,option);
				int i;
				for(i=0;i<10;i++)
					{	if(porr[i]>50)
						cout<<porr[i]<<" ";
						else
						break;
					}
				int sz=porr[i];
				cout<<sz<<" \n";

			int sockfd;
			sockfd = socket( AF_INET, SOCK_STREAM, 0);
			struct sockaddr_in    serv_addr;
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(porr[0]);
			serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
			if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
		    {printf("\nConnection Failed \n");
		    return NULL; }
		    
		
		  	send(sockfd,&a,sizeof(a),0);
		  	long double file_size;
			recv(sockfd, &file_size, sizeof(file_size), 0);
			cout<<file_size<<"\n";
			close(sockfd);

			long double chunks=file_size/(512*1024);
			cout<<chunks;






			
			
	}
	//pthread_exit(&ret);
	return NULL;

}
void * servercopy(void *arg)
{

	int ret=1;
	int sockfd= *((int *)arg);
	char a[512];
	recv(sockfd, &a, sizeof(a), 0);
	FILE *fp=fopen(a,"rb");
	fseek(fp,0,SEEK_END);
	long double size=ftell(fp);
	rewind(fp);
	send(sockfd,&size,sizeof(size),0);
	char Buffer[BUFF_SIZE] ; 
	long long n;
	while((n=fread(Buffer,sizeof(char),BUFF_SIZE,fp))>0 && size>0)
	{
			send(sockfd,Buffer,n,0);
	   	 	memset(Buffer,'\0',BUFF_SIZE);
			size=size-n;
	}
	///cout<<"Sending file complete\n";
	fclose(fp);
	close(sockfd);
	return NULL;
	//pthread_detach(pthread_self());
	//pthread_exit(&ret);

}

void  * server(void *yt)
{
	int server_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	int sockfd;
	struct sockaddr_in   addr;
	cout<<"ENTER server PORT NO:";
	cin>>serverport;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(serverport);
	addr.sin_addr.s_addr=INADDR_ANY;
	// cout<<"\nport no - "<<addr.sin_port<<"\n";

	int addrlen = sizeof(sockaddr);
	bind(server_fd,(struct sockaddr *)&addr,sizeof(addr));
	
	listen(server_fd,3);
	cout<<"LISTENING\n";
     pthread_t tid[6];
     int i = 0;
    while(1)
    {
    
    pthread_t t;
    addrlen=sizeof(addr);
	sockfd=accept(server_fd,(struct sockaddr *)&addr,(socklen_t*)&addrlen);
    if(pthread_create(&tid[i],NULL,&servercopy,&sockfd)!=0)
    printf("Failed to create thread\n");
		i++;
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
        pthread_t t;
		if(pthread_create(&t,NULL, &clientcopy,NULL)!=0)
		printf("Failed\n");
		pthread_join(t,NULL);
		// j++;

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