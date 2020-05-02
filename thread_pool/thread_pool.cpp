#include "./thread_pool.h"

bool ThreadPool::isFull(const int& index)
{
    int que_size = task_queue[index].size();
    int num_size = 10;
    return que_size > num_size ? true : false; 

}


bool ThreadPool::addTask(Task task, const int& index)
{

    std::unique_lock<std::mutex> mutex_lock((mutex_queue[index]));
    //判断是否已经满队了
    while(isFull(index))
    {
        std::cout<<"index: "<<index<<"task_queue is full"<<std::endl;
        //调用条件变量
        not_full[index].wait(mutex_lock);
    }

    task_queue[index].push_back(task);

    //只有一个线程使用，所以使用notify_one()
    not_empty[index].notify_one();


    return true;

}

void ThreadPool::executeTask(int index)
{
    while(is_running[index])
    {
        std::unique_lock<std::mutex> mutex_lock((mutex_queue[index]));

        //判断队列是否为空
        while(task_queue[index].empty())
        {
            std::cout<<"index: "<<index<<"task_queue is empty"<<std::endl;
            //调用条件变量
            not_empty[index].wait(mutex_lock);
        }
        //取任务队列首个任务
        if(task_queue[index].front() != NULL)
        {
            Task task = task_queue[index].front();
            task_queue[index].pop_front();
            //执行任务
            task();
        }
        not_full[index].notify_one();
    }

}

bool ThreadPool::start(const int& thread_num)
{
    num_of_thread = thread_num;
    //循环开启线程
    for(int i = 0; i < thread_num; i++)
    {

        thread_pool_vec.push_back(std::thread(&ThreadPool::executeTask, this, i));

        thread_pool_vec[i].detach();    //分离线程

        mutex_queue.push_back(new std::mutex());    

        is_running.push_back(true);

        not_empty.push_back(new std::condition_variable());

        not_full.push_back(new std::condition_variable());

        std::deque<Task> task_deque;

        task_queue.push_back(task_deque);

    }
    return true;
}

bool ThreadPool::stop(const int& index)
{
    //检查线程是否还在运行
    if(is_running[index] == false)return false;

    //停止线程
    is_running[index] = false;

    return true;
}