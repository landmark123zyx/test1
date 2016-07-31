//Shared reader-writer model:
//There is a shared data structure, and multiple threads access it.  If there is a reader,
//another thread can still read it, but no thread can write to it.  If there is a writer,
//no threads can read.
//A reader count needs needs to be maintained, and only if all the readers are done reading,
//can any thread write to the shared data structure.

#include<iostream>
#include<mutex>
#include<thread>
#include<memory>
#include<vector>
#include<assert.h>

using namespace std;

const int LOOP = 10, numReaders = 3, numWriters = 1;
bool task_done = false;
mutex m;
shared_ptr<vector<int>> ptr;

void reader(int num) {
  while(1)
  {
    if(task_done)
      return;

    { //temp_ptr will be destructed out of this block
      shared_ptr<vector<int>> temp_ptr;
      {
	//mutex among readers, readers and writer, 
	//but critical section is very small, so don't worry about the mutex
	//among readers.
        unique_lock<mutex> lk(m);
        temp_ptr=ptr;//ref count will be incremented
//int sz=temp_ptr.use_count();
//cout << "reader(), num=" << num << ", temp_ptr.use_count()=" << sz 
//<< ", temp_ptr->size()=" << temp_ptr->size() << endl;
        assert(!temp_ptr.unique());
      }
      //if there is a writer, then the reader is accessing old vector
      for(auto it=temp_ptr->begin();it!=temp_ptr->end();it++)
        cout<< *it<<" ";
      cout << endl;
    }
  }
}

void writer() {
  for(int i=0; i<LOOP; ++i) {
    if(task_done)
      return;

    {//any temp objs will be destructed out of this block
      //mutex among readers, readers and writer
      unique_lock<mutex> lk(m);
      //if there are other writers or readers,
      //then need to copy current vector
      if(!ptr.unique()) {
        ptr.reset(new vector<int>(*ptr));
      }
      assert(ptr.unique());
cout << i << " "; 
      ptr->push_back(i);
    }
  }
}

int main() {
  int i;
  thread rd[numReaders], wt[numWriters];
  ptr.reset(new vector<int>);
  for(i=0; i<numReaders; ++i)
    rd[i] = thread(reader, i);
  for(i=0; i<numWriters; ++i)
    wt[i] = thread(writer);
  
cout << "       before sleep() !!!" << endl;
  this_thread::sleep_for(std::chrono::seconds(3));
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
