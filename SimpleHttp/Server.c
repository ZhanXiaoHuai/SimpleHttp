#include"Server.h"
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<stdio.h>
#include <fcntl.h>
#include<errno.h>
#include<strings.h>
#include<string.h>

int initlistenfd(unsigned short port)
{
	//1.创建监听的fd
	int lfd = socket(AF_INET,SOCK_STREAM,0);
	if (lfd == -1) {
		perror("socket");
		return -1;
	}
	//2.设置端口复用
	int opt = 1;
	int ret=setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof opt);
	if (ret == -1){
		perror("setsockopt");
		return -1;
	}
	//3.绑定
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	ret=bind(lfd, (struct sockaddr*)&addr, sizeof addr);
	if (ret = -1) {
		perror("bind");
		return -1;
	}
	//4.监听
	ret = listen(lfd, 128);
	if (ret = -1) {
		perror("listen");
		return -1;
	}
	return lfd;
}

int epollrun(int lfd)
{
	//epoll本质是一个红黑树
	//1.创建epoll实例
	int epfd=epoll_create(1);
	if (epfd == -1) {
		perror("epoll_create");
		return -1;
	}
	//2.epoll上树
	struct epoll_event ev;	
	ev.data.fd = lfd;
	ev.events = EPOLLIN;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	if (ret == -1) {
		perror("epoll_ctl");
		return -1;
	}
	//3.检测
	struct epoll_event evs[1024];
	int size = sizeof(evs) / sizeof(struct epoll_event);
	while (1) {
		int num=epoll_wait(epfd, evs,sizeof size,-1);//所有唤醒的事件
		for (int i = 0; i < num; ++i) {
			int fd = evs[i].data.fd;
			if (fd == lfd) {
				//建立连接accept
				acceptClient(lfd,epfd);
			}
			else {//主要是接受读事件（EPOLLIN）

			}
		}
	}

	return 0;
}

int acceptClient(int lfd, int epfd)
{
	//1.建立连接
	int cfd = accept(lfd, NULL, NULL);
	if (cfd == -1) {
		perror("accept");
		return -1;
	}
	//2.设置为非阻塞
	int flag = fcntl(cfd,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(cfd,F_SETFL,flag);

	//3.将cfd添加到epoll树中
	struct epoll_event ev;
	ev.data.fd = cfd;
	ev.events = EPOLLIN|EPOLLET;//边缘触发模式
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
	if (ret == -1) {
		perror("epoll_ctl");
		return -1;
	}

	return 0;
}

int recvHttpRequest(int cfd, int epfd)
{

	int len = 0, totle = 0;
	char tmp[1024] = { 0 };
	char buf[4096] = { 0 };
	while ((len = recv(cfd, tmp, sizeof tmp, 0) > 0))
	{
		if (totle + len < sizeof buf)
		{
			memcpy(buf + totle, tmp, len);
		}
		totle += len;
	}
	//判断数据是否接收完毕
	if (len == -1 && errno == EAGAIN)
	{
		//解析请求行
	}
	else if (len == 0)
	{
		//客户端断开了连接
		epoll_ctl(epfd, EPOLL_CTL_DEL,cfd, NULL);
		close(cfd);
	}
	else
	{

	}
	return 0;
}

int parseRequestLine(const char* line, int cfd)
{
	// 解析请求行 get /xxx/1.jpg http/1.1
	char method[12];
	char path[1024];
	sscanf(line,"%[^ ] %[^ ]",method,path);//格式化匹配分割 使用正则表达式
	if (strcasecmp(method, "get") != 0)
	{
		return -1;
	}
	//处理客户端请求的静态资源(目标文件)

	return 0;
}
