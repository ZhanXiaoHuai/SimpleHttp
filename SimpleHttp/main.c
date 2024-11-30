#include"Server.h"

int main(int argc,char*argv[])
{
    if (argc < 3)
    {
        printf("./a.out port path\n");
        return -1;
    }
    unsigned short port = atoi(argv[1]);
    //初始化用于监听的套接字
    int lfd = initlistenfd(port);
    //启动服务器程序

    return 0;
}