#pragma once

//��ʼ���������׽���
int initlistenfd(unsigned short port);
//����epoll
int epollrun(int lfd);
//�Ϳͻ��˽�������
int acceptClient(int lfd, int epfd);
//����HTTP����
int recvHttpRequest(int cfd,int epfd);
//����������
int parseRequestLine(const char*line,int cfd);

