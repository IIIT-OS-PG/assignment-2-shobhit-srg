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
	int val;
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
		m[a].push_back(serverport);
		cout<<"file: "<<a<<"  port:  "<<serverport<<endl;
		memset ( a , '\0', 512);
	}

	if(option==1)
	{	char a[512];
		memset ( a , '\0', 512);
		recv(sockfd,&a,sizeof(a),0);
		vector<int> port;
			cout<<"a:"<<a<<endl;
			it=m.find(a);
		port=m[a];
		int sz=port.size();
		cout<<sz<<"    \n";
		for(auto i=0;i<sz;i++)
			cout<<port[i]<<" \n";
		///cout<<port[0]<<"\n";
		send(sockfd,&sz,sizeof(sz),0);

		int porr[sz];
		auto j=0;
		for(int i=0;i<sz;i++)
		{
			porr[i]=port[j];
			j++;
			send(sockfd,&porr[i],sizeof(porr[i]),0);
		}
		// send(sockfd,&port[0],sizeof(port[0]),0);
		// send(sockfd,&port[1],sizeof(port[1]),0);
		//for(auto j=0;j<sz;j++)
			// {	
			// 	val=port[j];
			// 	cout<<port[j]<<" ";
			// 	send(sockfd,&port[j],sizeof(port[j]),0);
			// }
			// cout<<port[i]<<" ";
			// cout<<endl;
		//cout<<"x:"<<x<<endl;
		
		memset ( a , '\0', 512);
	}
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