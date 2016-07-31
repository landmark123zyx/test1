//Shared reader-writer model:
//There is a shared data structure, and multiple threads access it.  If there is a reader,
//another thread can still read it, but no thread can write to it.  If there is a writer,
//no threads can read.
//A reader count needs to be maintained, and only if all the readers are done reading,
//can any thread write to the shared data structure.

//shared_mutex is not supported in C++14 yet,
//so, use shared_timed_mutex

#include<iostream>
#include<thread>
#include<shared_mutex>
#include<condition_variable>
#include<memory>
#include<vector>
#include<assert.h>
#include <chrono>

using namespace std;

const int LOOP = 10, numReaders = 5, numWriters = 1;
bool task_done = false;
shared_ptr<vector<int>> ptr;

void reader(int num, shared_timed_mutex &stm)
{
  while (1)
  {
    if (task_done) {
      return;
    }

    {
      //requires shared ownership to read
      shared_lock<shared_timed_mutex> lk(stm);

      //reading...
      for (auto it=ptr->begin();it!=ptr->end();it++)
        cout<< *it<<" ";
    } // will unlock by the lk dtor
  }
}

void writer(shared_timed_mutex &stm)
{
  for (int i=0; i<LOOP; ++i)
  {
    if(task_done) {
      return;
    }

    {
      //requires exclusive ownership to write 
      unique_lock<shared_timed_mutex> lk(stm);

      //writing...
      ptr->push_back(i);
    } // will unlock by the lk dtor
  }
}

int main() {
  int i, numReadersWaiting = 0;
  bool writeWaiting = false;
  shared_timed_mutex stm;
  thread rd[numReaders], wt[numWriters];
  ptr.reset(new vector<int>);
  for(i=0; i<numReaders; ++i)
    rd[i] = thread(reader, i, ref(stm));
  for(i=0; i<numWriters; ++i)
    wt[i] = thread(writer, ref(stm));
  
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
