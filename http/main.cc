#include <iostream>
#include "curl/curl.h"
#include "thread_pool.h"
#include "http_conn.h"
#include "mylib_ini.h"



const int MAX_FD = 65536;
const int MAX_EVENT_NUMBER = 10000;

extern void addfd(int epollfd, int fd, bool one_shot);
extern void removefd(int epollfd, int fd);

void addsig(int sig, void(handler)(int), bool restart = true)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    if(restart)
    {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

void show_error(int connfd, const char* info)
{
    printf("%s\n", info);
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int main(int argc, char * argv[])
{
    if(argc <= 1)
    {
        printf("usage: %s ip_address port_number\n", argv[0]);
        return 1;
    }

    std::string config_file(argv[1]);
    printf("config file:%s\n", config_file.c_str());
    mylib::MylibIni config;
    config.Init(config_file);

    std::string ip_str = config.get("srv", "ip", "0.0.0.0");
    int port = config.get("srv", "port", 8080);

    //const char* ip = argv[1];
    //int port = atoi(argv[2]);
    const char *ip = ip_str.c_str();

    printf("ip:%s, port:%d\n", ip, port);
   

    /**/
    // addsig(SIGPIPE, SIG_IGN);

    // mylib::ThreadPool<HttpConn> *pool = nullptr;
    // try
    // {
    //     pool = new mylib::ThreadPool<HttpConn>;
    // }
    // catch(...)
    // {
    //     return 2;
    // }
    // HttpConn * users = new HttpConn[MAX_FD];
    // assert(users);
    // //int user_count = 0;

    // int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    // assert(listenfd >= 0);
    // struct linger tmp = {1, 0};
    // setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));

    // int ret = 0;
    // struct sockaddr_in address;
    // bzero(&address, sizeof(address));
    // address.sin_family = AF_INET;
    // inet_pton(AF_INET, ip, &address.sin_addr);
    // address.sin_port = htons(port);

    // ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    // assert(ret >= 0);

    // ret = listen(listenfd, 5);
    // assert(ret >= 0);

    // epoll_event events[MAX_EVENT_NUMBER];
    // int epollfd = epoll_create(5);
    // assert(epollfd !=-1);
    // addfd(epollfd, listenfd, false);

    // HttpConn::epollfd_ = epollfd;

    // for(;;)
    // {
    //     int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    //     if((number < 0) && (errno != EINTR))
    //     {
    //         printf("%s\n", "epoll failure");
    //         break;
    //     }
    //     for(int i = 0; i < number; ++i)
    //     {
    //         int sockfd = events[i].data.fd;
    //         printf("sockfd:%d, listenfd:%d\n", sockfd, listenfd);
    //         if(sockfd == listenfd)
    //         {
    //             struct sockaddr_in client_address;
    //             socklen_t client_addr_len = sizeof(client_address);
    //             int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addr_len);
    //             printf("connfd:%d\n",connfd);
    //             if(connfd < 0)
    //             {
    //                 printf("errno is:%d\n", errno);
    //                 continue;
    //             }
    //             if(HttpConn::user_count_ >= MAX_FD)
    //             {
    //                 show_error(connfd, "Internal server busy");
    //                 continue;
    //             }
    //             /*初始化客户端连接*/
    //             users[connfd].init(connfd, client_address);
    //         }
        
    //         else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
    //         {
    //             /*如果有异常，关闭连接*/
    //             users[sockfd].close_conn();
    //         }
    //         else if(events[i].events & EPOLLIN)
    //         {
    //             printf("add pool \n");
    //             /*根据读取的结果，决定是将任务添加到线程池，还是关闭连接*/
    //             if(users[sockfd].read())
    //             {
    //                 pool->append(users + sockfd);
    //             }
    //             else
    //             {
    //                 users[sockfd].close_conn();
    //             }
    //         }
    //         else if (events[i].events & EPOLLOUT)
    //         {
    //              printf("write \n");
    //             /*根据写的结果，决定是否关闭连接*/
    //             if(!users[sockfd].write())
    //             {
    //                 users[sockfd].close_conn();
    //             }
    //         }
    //         else
    //         {}
    //     }
    // }
    // close(epollfd);
    // close(listenfd);
    // delete [] users;
    // delete pool;
    return 0;
}