//Producer and consumer model.

#include<iostream>
#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>
#include <chrono>

using namespace std;

const int maxQ=4; 
const int numProducer=3; 
const int numConsumer=5;
bool task_done=false;
mutex m;
condition_variable cv;
queue<int> que;

void consume(int idx)
{
  while(1)
  {
    unique_lock<mutex> lk(m);
//    if(!lk.owns_lock()) {
//      //never goes here
//cout << "  consume(), idx=" << idx << ", NOT owns lock! lock it." << endl; 
//      lk.lock();
//    }

/***
    while(que.empty() && !task_done) {
      //lk.unlock() will be called when calling wait
//      cv.wait(lk); //cannot detect 'task_done'
      cv.wait_for(lk, chrono::seconds(2));
    }
***/
    //or, use std::condition_variable 2nd wait_for(), don't need loop,
    //use lambda as pred
    cv.wait_for(lk, chrono::seconds(2), [&] { return !que.empty() || task_done; });

    if(task_done) {
//      if(lk.owns_lock())
//        lk.unlock();
      return;
    }

    int num = que.front();
    que.pop();
//cout << "consume(), idx=" << idx << endl;
    cv.notify_all();
    //lk.lock() will be called when a blocked thread is waken up.
//cout << "consume(), idx=" << idx << ", aft cv.notify_all()" << endl;
  }
}

void produce(int idx)
{
  while(1)
  {
    unique_lock<mutex> lk(m);
//    if(!lk.owns_lock()) {
//      //never goes here
//cout << "  produce(), idx=" << idx << ", NOT owns lock! lock it." << endl; 
//      lk.lock();
//    }

    while(que.size() >= maxQ && !task_done) {
      //lk.unlock() will be called when calling wait
//      cv.wait(lk); //cannot detect 'task_done'
      cv.wait_for(lk, chrono::seconds(2));
    }

    if(task_done) {
//      if(lk.owns_lock())
//        lk.unlock();
      return;
    }

    que.push(idx);
//cout << "produce(), idx=" << idx << " pushed into que" << endl;
    cv.notify_all();
    //lk.lock() will be called when a blocked thread is waken up.
//cout << "  produce(), idx=" << idx << ", aft cv.notify_all()" << endl;
  }
}

int main(int argc,char* argv[]) {
  int i;
  thread p[numProducer], c[numConsumer];
  for(i=0; i<numProducer; ++i)
    p[i] = thread(produce, i);
  for(i=0; i<numConsumer; ++i)
    c[i] = thread(consume, i);

  this_thread::sleep_for(std::chrono::seconds(3));
cout << "	after sleep() !!!" << endl;
  task_done = true;

  for (auto& th: p) th.join();
cout << " aft p[] join" << endl;
  for(i=0; i<numConsumer; ++i) {
    c[i].join();
cout << " aft c[" << i << "] join" << endl;
  }

  return 0;
}
