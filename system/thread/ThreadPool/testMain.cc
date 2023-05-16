#include "threadPool.hpp"
#include "Task.hpp"
#include <iostream>

int main()
{
    ThreadPool<Task>* tp = new ThreadPool<Task>();
    tp->run();
    tp->joins();
    
    // while (true)
    // {}

    return 0;
}