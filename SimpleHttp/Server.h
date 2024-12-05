#pragma once

//��ʼ���������׽���
int initListenFd(unsigned short port);
//����epoll
int epollRun(int lfd);
//�Ϳͻ��˽�������
int acceptClient(int lfd, int epfd);
//����HTTP����
int recvHttpRequest(int cfd,int epfd);
//����������
int parseRequestLine(const char*line,int cfd);
//�����ļ�
int sendFile(const char*filename,int cfd);
// ������Ӧͷ(״̬��+��Ӧͷ)
int sendHeadMsg(int cfd, int status, const char* descr, const char* type, int length);
//�õ�content-type
const char* getFileType(const char* name);
//����Ŀ¼
int sendDir(const char* dirName, int cfd);