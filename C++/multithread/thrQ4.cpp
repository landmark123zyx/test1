//Give code that:
//start 3 threads, their ID are A, B, C, respectively.
//each thread prints its ID (on screen) 10 times,
//the output sequence must be ABC, like: ABCABC..., etc.

#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

using namespace std;

mutex m;
condition_variable cv;
int LOOP=10;
int counter=0;

void fun(int id){
  for(int i=0;i<LOOP;i++){
    unique_lock<mutex> lk(m);

//    while(id!=counter)//must use while loop, to prevent from "Spurious wakeup"
//      cv.wait(lk);
    //or, use std::condition_variable 2nd wait(), don't need loop,
    //use lambda as pred
    cv.wait(lk, [&] { return id==counter; });

    cout<<(u_char)('A'+id)<<" ";
    counter=(counter+1)%3;
    cv.notify_all();
  }
}

int main(){
/***
    thread B(fun,1);
    thread C(fun,2);
    fun(0);
    cout<<endl;
    B.join();
    C.join();
***/

  thread A(fun, 0);
  thread B(fun, 1);
  thread C(fun, 2);
  A.join();
  B.join();
  C.join();
  return 0;
}
