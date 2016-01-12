//Give the code that:
//1. child thread loops 10 times, then;
//2. main thread loops 100 times;
//3. repeat step 1 & 2, 50 times.

#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

using namespace std;

//@ ttttt:
//testing: updated on remote Github...

mutex m;
condition_variable cond;
int flag=10;

void fun(int num) {
  for(int i=0;i<50;i++){
    unique_lock<mutex> lk(m);//A unique lock is an object that manages
			     //a mutex object with unique ownership
			     //in both states: locked and unlocked.
    while(flag!=num) //cannot use if(), to prevent from "Spurious wakeup"
      cond.wait(lk);//lk.unlock() will be called when calling wait
    for(int j=0;j<num;j++)
      cout<<j<<" ";
    cout<<endl;
    flag=(num==10)?100:10;
    cond.notify_one();//lk.lock() will be called when a blocked thread is waken up.
  }
}

int main(){
  thread child(fun,10);
  fun(100);
  child.join();
  return 0;
}
