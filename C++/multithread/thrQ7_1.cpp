//Shared reader-writer model:
//There is a shared data structure, and multiple threads access it.  If there is a reader,
//another thread can still read it, but no thread can write to it.  If there is a writer,
//no threads can read.
//A reader count needs needs to be maintained, and only if all the readers are done reading,
//can any thread write to the shared data structure.

#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<memory>
#include<vector>
#include<assert.h>
#include <chrono>

using namespace std;

const int LOOP = 10, numReaders = 3, numWriters = 1;
bool task_done = false;
shared_ptr<vector<int>> ptr;

void reader(int num, mutex &m, condition_variable &cv,
	int &numReadersWaiting, bool &writerWaiting)
{
  while(1)
  {
    unique_lock<mutex> lk(m);
    while(writerWaiting && !task_done) {
      //lk.unlock() will be called when calling wait
      cv.wait_for(lk, chrono::seconds(1));
    }  

    if(task_done) {
      return;
    }

    ++numReadersWaiting;
    //reading...
    for(auto it=ptr->begin();it!=ptr->end();it++)
      cout<< *it<<" ";
    --numReadersWaiting;

    cv.notify_all();
    //lk.lock() will be called when a blocked thread is waken up.
  }
}

void writer(mutex &m, condition_variable &cv,
	int &numReadersWaiting, bool &writerWaiting)
{
  for(int i=0; i<LOOP; ++i)
  {
    unique_lock<mutex> lk(m);
    while ((writerWaiting || numReadersWaiting > 0) && !task_done)
    {
      //lk.unlock() will be called when calling wait
      cv.wait_for(lk, chrono::seconds(1));
    }

    if(task_done) {
      return;
    }

    writerWaiting = true;
    //writing...
    ptr->push_back(i);
    writerWaiting = false;

    cv.notify_all();
    //lk.lock() will be called when a blocked thread is waken up.
  }
}

int main() {
  int i, numReadersWaiting = 0;
  bool writeWaiting = false;
  mutex m;
  condition_variable cv;
  thread rd[numReaders], wt[numWriters];
  ptr.reset(new vector<int>);
  for(i=0; i<numReaders; ++i)
    rd[i] = thread(reader, i, ref(m), ref(cv), ref(numReadersWaiting), ref(writeWaiting));
  for(i=0; i<numWriters; ++i)
    wt[i] = thread(writer, ref(m), ref(cv), ref(numReadersWaiting), ref(writeWaiting));
  
cout << "       before sleep() !!!" << endl;
  this_thread::sleep_for(std::chrono::seconds(1));
  task_done = true;
cout << "       after sleep() !!!" << endl;
  for (auto& th: rd) th.join();
cout << " aft rd[] join" << endl;
  for(i=0; i<numWriters; ++i) {
    wt[i].join();
cout << " aft wt[" << i << "] join" << endl;
  }

  return 0;
}
