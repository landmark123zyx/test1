#include<iostream>
#include<fstream>
#include<thread>
#include<mutex>
#include<condition_variable>

//有四个线程1、2、3、4。线程1的功能就是输出1，线程2的功能就是输出2，以此类推.........
//现在有四个文件ABCD。初始都为空。现要让四个文件呈如下格式：
//A：1 2 3 4 1 2....
//B：2 3 4 1 2 3....
//C：3 4 1 2 3 4....
//D：4 1 2 3 4 1....
//请设计程序。

using namespace std;

const int LOOP=12;
mutex m;
condition_variable cv;
int cnt = 0;
int numArr[4] = {1, 2, 3, 4};

void fun(ofstream *ofPtr, int num)
{
  for(int i=0;i<LOOP;i++){
    unique_lock<mutex> lk(m);
//cout << "cnt=" << cnt << ", num+1=" << num+1 << endl;
    while(cnt!=num)
      cv.wait(lk);
    *ofPtr << numArr[num] <<" ";
    cnt=(cnt+1)%4;
    numArr[num] = (numArr[num]+1)%4;
    if(numArr[num] == 0)
      numArr[num] = 4;
    cv.notify_all();
  }
}

int main(int argc,char* argv[]) {
  ofstream fileA("A.txt", ios::out);
  ofstream fileB("B.txt", ios::out);
  ofstream fileC("C.txt", ios::out);
  ofstream fileD("D.txt", ios::out);

  thread tb(fun, &fileB, 1);
  thread tc(fun, &fileC, 2);
  thread td(fun, &fileD, 3);
  fun(&fileA, 0);
  tb.join();
  tc.join();
  td.join();
  fileA.close();
  fileB.close();
  fileC.close();
  fileD.close();

  return 0;
}
