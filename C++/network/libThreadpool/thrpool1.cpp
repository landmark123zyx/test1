#include <iostream>
//#include <atomic.h>

#include "thrpool1.h"

// the constructor just launches some amount of workers
//inline ThreadPool::ThreadPool(size_t threads)
ThreadPool::ThreadPool(size_t threads)
//    : std::atomic_init<size_t>(workingItemNumbers, 0), stop(false)
    : stop(false)
{
  for(size_t i = 0;i<threads;++i)
    workers.emplace_back(
      [this]
      {
	for(;;) {
          std::function<void()> task;
          {
  {
unique_lock<mutex> locker(lockcout88);
std::cout << " tid: " << this_thread::get_id() << ", bef lock(queue_mutex), tasks.sz=" 
 << tasks.size() << ", stop=" << this->stop << '\n';
  }
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(lock,
		[this]{ return this->stop || !this->tasks.empty(); });
            if(this->stop && this->tasks.empty())
              return;
            task = std::move(this->tasks.front());
            this->tasks.pop();
  {
unique_lock<mutex> locker(lockcout88);
std::cout << " tid: " << this_thread::get_id() << ", aft dequeue tasks, tasks.sz=" 
 << tasks.size() << ", stop=" << this->stop << '\n';
  }
          }

	  workingItemNumbers++;
  {
unique_lock<mutex> locker(lockcout88);
std::cout << " tid: " << this_thread::get_id() << ", bef task(), workingItemNumbers="
	<< workingItemNumbers.load() << '\n';
  }
	  task();
	  workingItemNumbers--;
  {
unique_lock<mutex> locker(lockcout88);
std::cout << " tid: " << this_thread::get_id() << ", aft task(), workingItemNumbers="
	<< workingItemNumbers.load() << '\n';
  }
        } //for(;;)
      }
    );
}

// the destructor joins all threads
//will cause compiling err as:
///home/peter/src/c++/network/libThreadpool/thrpoolTest.cpp:7: undefined reference to `ThreadPool::~ThreadPool()'
//inline ThreadPool::~ThreadPool()
ThreadPool::~ThreadPool()
{
  {
unique_lock<mutex> locker(lockcout88);
std::cout << " tid: " << this_thread::get_id() << ", enter dtor." << '\n';
  }

  //RAII
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  condition.notify_all();
  for(std::thread &worker: workers)
    worker.join();
}
