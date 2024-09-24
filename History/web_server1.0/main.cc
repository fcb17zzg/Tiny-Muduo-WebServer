#include "address.h"
#include "echo.h"

using namespace tiny_muduo;

/*
    1、EchoServer（最简单的echo server 有两个回调函数）
    2、EventLoop（核心部分 命令一直都在循环在Epoll_wait 执行任务）
    3、TcpServer（最核心的部分 管控着整个服务器）
    4、Acceptor（负责新连接 也只负责连接新连接 其他的多余的不管）
    5、Channel（跑腿工 所以最基本的任务 也就是Channel负责在各个组件中来回奔波
   Channel核心是在于他的回调函数 可注册Epollfd 在事件触发时可调用回调函数）
    6、TcpConnectionPtr（Send Recv 工作是在这里做的 每创建一个连接
   也就会生成一个这样的Ptr
   EchoServer与每个客户端联系也就是通过TcpConnectionPtr来进行沟通 Send
   Recv基础任务由它底层实现） 7、Epoller（创建Epollfd 注册Epoll事件
   封装Epoll_wait 关于Epoll的一切都封装在这个类里面 Channel每次获得新连接
   也就会调用Epoller的Update() 把自己放进去）
    8、Thread\ThreadPool\EventLoopThreadPool（暂不支持多线程
   目前工作模式仅支持单线程Reactor 之后这一版出了之后 会马上在其基础上
   实现有关多线程的类 敬请期待…）
*/

int main(int argc, char *argv[]) {
    EventLoop loop;
    Address listen_address(argv[2]);
    EchoServer server(&loop, listen_address);
    server.Start();
    loop.Loop();
    return 0;
}
