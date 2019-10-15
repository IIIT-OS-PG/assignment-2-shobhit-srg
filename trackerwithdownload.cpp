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

map<string, vector<int> > m;
map<string, vector<int> >::iterator it;

void * tracker(void *arg)
{

	cout<<"READY\n";
	int sockfd= *((int *)arg);
	int option;
	recv(sockfd,&option,sizeof(option),0);
	if(option==0)
	{
		char a[512];
		memset ( a , '\0', 512);
		recv(sockfd,&a,sizeof(a),0);
		int serverport;
		recv(sockfd,&serverport,sizeof(serverport),0);
		m.insert({a,serverport});
		cout<<"file: "<<a<<"  port:  "<<serverport<<endl;
		memset ( a , '\0', 512);
	}

	if(option==1)
	{	char a[512];
		memset ( a , '\0', 512);
		recv(sockfd,&a,sizeof(a),0);
		int x;
			cout<<"a:"<<a<<endl;
			auto it=m.find(a);
		x=it->second;
		cout<<"x:"<<x<<endl;
		send(sockfd,&x,sizeof(x),0);
		memset ( a , '\0', 512);}

	
	

	close(sockfd);

}

void  * server(void *yt)
{
	int server_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int sockfd;
	struct sockaddr_in   addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr=INADDR_ANY;
	int addrlen = sizeof(sockaddr);
	bind (server_fd,(struct sockaddr *)&addr,sizeof(addr));
	listen(server_fd,3);
	cout<<"LISTENING\n";

	while(1)
    {
    
    pthread_t t;
    addrlen=sizeof(addr);
	sockfd = accept ( server_fd , (struct sockaddr *)&addr,(socklen_t*)&addrlen);
    if( pthread_create(&t, NULL, &tracker, &sockfd) != 0 )
    printf("Failed to create thread\n");
    }

	///close( sockfd);	
	close(server_fd);
	return NULL;
}
int main()
{

			pthread_t sid;
			if(pthread_create(&sid,NULL,&server,NULL)!=0)
				cout<<"failed";

				pthread_join(sid,NULL);
	
}