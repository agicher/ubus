#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <assert.h>


#define MAXBUF 1024
#define MAXEFOLLSIZE 100

int set_non_block(int fd)
{
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK) == -1)
    {
        return -1;
    }
    return 0;
    
}

void * pthread_handle_message(int *fd)
{
    char rcvbuf[MAXBUF + 1];
    char sndbuf[MAXBUF + 1];

    int ret;
    int new_fd;
    struct sockaddr_in cli_addr;
    socklen_t sock_len = sizeof(struct sockaddr_in);

    new_fd = *fd;
    
    __bzero(rcvbuf, MAXBUF + 1);
    __bzero(sndbuf, MAXBUF + 1);

    ret = recvfrom(new_fd, rcvbuf, MAXBUF, 0, (struct sockaddr *)&cli_addr, &sock_len);

    if (ret > 0)
    {
        printf("socket %d 接收到来自:%s:%d的消息成功:’%s’，共%d个字节的数据/n",
        new_fd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), rcvbuf, ret);
    }
    else
    {
        printf("消息接收失败！错误代码是%d，错误信息是’%s’/n",
        errno, strerror(errno));
    }

    /* 处理每个新连接上的数据收发结束 */ 
    //printf("pthread exit!");
    fflush(stdout); 
    pthread_exit(NULL);
    
}

int main(int argc, char **argv)
{
    int listener, kdpfd, n ,curfds;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int port;
    struct epoll_event ev;
    struct epoll_event events[MAXEFOLLSIZE];
    struct rlimit rt;

    port = 1234;

    pthread_t thread;
    pthread_attr_t attr;

    rt.rlim_max = rt.rlim_cur = MAXEFOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
    {
        perror("setrlimit");
        exit(1);
    }

    int opt=SO_REUSEADDR;
    setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    setnonblocking(listener);
    __bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listener, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) 
    {
        perror("bind");
        exit(1);
    } 
    

    kdpfd = epoll_create(MAXEFOLLSIZE);
    len = sizeof(struct sockaddr_in);
    ev.events = listener;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0)
    {
        return -1;
    }


    while (1)
    {
        int n;
        events[n].data.fd;
    }
    
    


}