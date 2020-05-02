#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <deque>
#include <functional>
#include <condition_variable>
#include <boost/ptr_container/ptr_vector.hpp>

typedef std::function<void()> Task;             //任务的模板

class ThreadPool
{

private:

    std::vector<std::thread>  thread_pool_vec;      //线程池的容器

    std::vector<std::deque<Task>>   task_queue;     //任务队列

    boost::ptr_vector<std::mutex>   mutex_queue;    //读写锁队列

    std::vector<bool>   is_running;                 //运行标志

    // std::mutex* mutex_queue;                        //读写锁队列

    // std::condition_variable* not_empty;             //非空条件变量

    // std::condition_variable* not_full;              //非满变量

    boost::ptr_vector<std::condition_variable>  not_empty;  //非空条件变量

    boost::ptr_vector<std::condition_variable>  not_full;   //非满变量

    int num_of_thread;                              //线程的数量

public:

    ThreadPool(const int& thread_num)
    {
        //根据thread_num初始化线程池的大小
        if(thread_num > 0)
        {
            thread_pool_vec.reserve(thread_num);
            task_queue.reserve(thread_num);
            mutex_queue.reserve(thread_num);
            not_empty.reserve(thread_num);
            not_full.reserve(thread_num);
            start(thread_num);
        }
    }

    ~ThreadPool()
    {
        // delete []mutex_queue;
        // delete []not_empty;
        // delete []not_full;
        // mutex_queue = NULL;
        // not_empty = NULL;
        // not_full = NULL;
    }

    //放任务的函数
    bool addTask(Task task, const int& index);

private:

    //运行任务的函数
    void executeTask(int index);

    //启动线程的函数
    bool start(const int& thread_num);

    //终止线程的函数
    bool stop(const int& index);

    bool isFull(const int& index);

};


#endif
