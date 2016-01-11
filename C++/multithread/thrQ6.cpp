//生产者消费者问题
//这是一个非常经典的多线程题目，题目大意如下：有一个生产者在生产产品，
//这些产品将提供给若干个消费者去消费，为了使生产者和消费者能并发执行，
//在两者之间设置一个有多个缓冲区的缓冲池，生产者将它生产的产品放入一个缓冲区中，
//消费者可以从缓冲区中取走产品进行消费，所有生产者和消费者都是异步方式运行的，
//但它们必须保持同步，即不允许消费者到一个空的缓冲区中取产品，也不允许生产者
//向一个已经装满产品且尚未被取走的缓冲区中投放产品。
//分析：假设1个生产者，2个消费者，缓冲区大小为4。
//第一．从缓冲区取出产品和向缓冲区投放产品必须是互斥进行的。可以用关键段和互斥量来完成。
//第二．生产者要等待缓冲区为空，这样才可以投放产品，消费者要等待缓冲区不为空，
//这样才可以取出产品进行消费。并且由于有二个等待过程，所以要用二个事件或信号量来控制。

#include<iostream>
#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>
#include <chrono>
#include <unistd.h> // sleep()

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
  unique_lock<mutex> lk(m);
  while(1)
  {
    if(!lk.owns_lock()) {
      //never goes here
//cout << "  consume(), idx=" << idx << ", NOT owns lock! lock it." << endl; 
      lk.lock();
    }

    while(que.empty() && !task_done)
//      cv.wait(lk); //cannot detect 'task_done'
      cv.wait_for(lk, chrono::seconds(2));

    if(task_done) {
      if(lk.owns_lock())
        lk.unlock();
      return;
    }

    int num = que.front();
    que.pop();
//cout << "consume(), idx=" << idx << endl;
    cv.notify_all();
//cout << "consume(), idx=" << idx << ", aft cv.notify_all()" << endl;
  }
}

void produce(int idx)
{
  unique_lock<mutex> lk(m);
  while(1)
  {
    if(!lk.owns_lock()) {
      //never goes here
//cout << "  produce(), idx=" << idx << ", NOT owns lock! lock it." << endl; 
      lk.lock();
    }

    while(que.size() >= maxQ && !task_done)
//      cv.wait(lk); //cannot detect 'task_done'
      cv.wait_for(lk, chrono::seconds(2));

    if(task_done) {
      if(lk.owns_lock())
        lk.unlock();
      return;
    }

    que.push(idx);
//cout << "produce(), idx=" << idx << " pushed into que" << endl;
    cv.notify_all();
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

  sleep(3);
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
