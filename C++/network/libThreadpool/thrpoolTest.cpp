#include <mutex>
#include <iostream>
#include <unistd.h>

#include "thrpool1.h"

using namespace std;

void func1(int thrIdx, ThreadPool *pl) {
//  cout << "  thrIdx=" << thrIdx << " going to sleep..." << '\n';
//  sleep(20);
//  cout << "  thrIdx=" << thrIdx << " wakes" << '\n';
  {
    unique_lock<mutex> locker(pl->lockcout88);
    cout << "  tid: " << this_thread::get_id() << " going to sleep..." << '\n';
  }

  std::this_thread::sleep_for(std::chrono::seconds(20));

  {
    unique_lock<mutex> locker(pl->lockcout88);
    cout << "  tid: " << this_thread::get_id() << " wakes" << '\n';
  }
}

int main() {
/***
  // create thread pool with 4 worker threads
  ThreadPool pool(4);

  // enqueue and store future
  auto result = pool.enqueue([](int answer) { return answer; }, 42);

  // get result from future
  std::cout << result.get() << std::endl;
***/

  ThreadPool pool(2);

  for(int i=0; i<3; ++i) {
    {
unique_lock<mutex> locker(pool.lockcout88);
    cout << "loop: " << i << '\n'; 
    }
//    auto result = pool.enqueue([=](int answer) { return answer; }, i+42);
//    cout << result.get() << '\n';
    auto result = pool.enqueue(func1, i, &pool);
  }

  std::this_thread::sleep_for(std::chrono::seconds(60));

  return 0;
}
