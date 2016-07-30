#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
//testing
#include <boost/type_index.hpp>

using namespace std;



class ThreadPool {
 public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
//equivalent to:
//  std::future<typename std::result_of<F(Args...)>::type> enqueue(F&& f, Args&&... args); 

    ~ThreadPool();

  size_t getWorkingItemNumbers() {return workingItemNumbers.load();}
//for testing:
mutex lockcout88; 

 private:
  // need to keep track of threads so we can join them
  std::vector< std::thread > workers;
  // the task queue
  std::queue< std::function<void()> > tasks;
  atomic<size_t> workingItemNumbers = {0};
    
  // synchronization
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};

//cannot put template code in .cpp file, compiling err:
//thrpool1.h:17:58: error: ‘std::future<typename std::result_of<_Functor(_ArgTypes ...)>::type> ThreadPool::
//enqueue(F&&, Args&& ...) [with F = main()::<lambda(int)>&; Args = {int}; typename std::
//result_of<_Functor(_ArgTypes ...)>::type = int]’, declared using local type ‘main()::<lambda(int)>’,
// is used but never defined [-fpermissive]
//   std::future<typename std::result_of<F(Args...)>::type> enqueue(F&& f, Args&&... args);
//                                                          ^~~~~~~
//------------------------------
// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
  using return_type = typename std::result_of<F(Args...)>::type;
  //return_type -- void

  auto task = std::make_shared< std::packaged_task<return_type()> >(
	std::bind(std::forward<F>(f), std::forward<Args>(args)...));

 {
  unique_lock<mutex> locker(lockcout88);
  using boost::typeindex::type_id_with_cvr;
  cout << "type of task: " << type_id_with_cvr<decltype(task)>().pretty_name() << "'\n";
  //type of task: std::shared_ptr<std::packaged_task<void ()> >
 }

  std::future<return_type> res = task->get_future();
  {
    {
      unique_lock<mutex> locker(lockcout88);
cout << "  tid: " << this_thread::get_id() << ", going to tasks queue, tasks.sz=" << tasks.size() << '\n';
    }
    std::unique_lock<std::mutex> lock(queue_mutex);

    // don't allow enqueueing after stopping the pool
    if(stop)
      throw std::runtime_error("enqueue on stopped ThreadPool");

    tasks.emplace([task](){ (*task)(); });

    {
      unique_lock<mutex> locker(lockcout88);
cout << "  tid: " << this_thread::get_id() << ", aft enq tasks queue, tasks.sz=" << tasks.size() << '\n';
    }
  }
  condition.notify_one();
  return res;
}
#endif
