# High-Performance-WebServer

本项目是基于C++11编写的Web服务器，整个服务器是以事件驱动模式为核心。  
支持解析HTTP头部，响应HTTP GET请求，可支持服务器长连接。  
实现了服务器定时断开空闲连接（基于内部定时器实现），异步高性能日志库完成服务器Logging功能。  

## System Requires:

  Linux kernel version >= 2.6.28.

  支持Ubuntu/CentOS/Debian, etc.  
  编写环境于Ubuntu 20.04 g++ 4.8.5

## Packages Required:

  `g++ >= 4.8.5`  
  `cmake`  
  `make`  

  **Debian, Ubuntu, etc.**  
  `sudo apt install g++ cmake make`
    
  **CentOS**  
  `sudo yum install gcc-c++ cmake make`

## To Build

	./build.sh

## Optimized Methods

  * 采用Epoll I/O多路复用工作函数，工作模式采用LT水平触发模式，对长连接短连接处理性能友好  
  * 采用RAII机制，智能指针Shared/Unique Ptr来管理内存，避免手动delete，同时也避免了可能存在的内存泄漏问题  
  * 采用线程池管理线程，并在程序开始时产生线程，避免程序运行中产生影响性能，并具有可预测性的性能  
  * 采用Round-Robin算法，简单高效的为子工作线程分发任务，实现了简单的负载均衡  
  * 采用DOUBLE-BUFFERING双缓冲机制，自定义固定大小Buffer并重载运算符，优化异步日志库写入性能  
  * 采用Eventfd作为事件通知描述符，效率高且非常简单的通信工具，优化主线程和子线程间的事件通知效率  
  * 采用Timerfd作为定时器触发描述符，红黑树作为定时器管理基本数据结构，高效的管理定时器触发事件，完美的融入了事件驱动管理  
  * 采用__thread gcc内置线程局部存储设施，存储日期/PID/时间，节省了多次系统调用，进一步优化日志库Logging效率  
  * 采用优雅断开服务器连接方式，避免直接Close而丢失双方仍未发出或接受的数据  
  * 采用C++11 std::move移动函数避免大量不必要的拷贝动作  
  * 采用提前准备一个空的文件描述符，在面对服务器文件描述符耗尽时关闭/接受/再关闭的方法，有效避免因描述符耗尽而导致服务器Busy Loop  
  * 采用有限状态机分析TCP字节流是否为HTTP包，高效的解析HTTP请求中的URI以及HTTP头部参数，采用unordered_map来记录参数方便使用  
  * WebServer采用One Loop Per Thread + Thread Pool编程模式，其内部实现了一个小型Reactor模式的网络库

## Code Statistics
  ![image](https://user-images.githubusercontent.com/72536813/164964817-6aa10adb-0519-411c-982d-c853181f89cb.png)
