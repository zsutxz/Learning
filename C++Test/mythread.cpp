#include <thread>
#include <atomic>
#include <stdio.h>
#include <string>

#include <iostream>
#include <vector>
#include <list>

using namespace std; 

atomic_bool bIsReady(false);
atomic_int iCount(100);

void threadfun1()
{
    cout << "threadfun1 - 1\r\n" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "threadfun1 - 2" << endl;
}

void threadfun2(int iParam, string sParam)
{
    cout << "threadfun2 - 1" << endl;
    this_thread::sleep_for(chrono::seconds(5));
    cout << "threadfun2 - 2" << endl;
}


void threadfun12()
{
    if(!bIsReady)
    {
        this_thread::yield();
    }

    while(iCount>0)
    {
        cout<<"iCount:"<<iCount--<<endl;
    }
}

int test_thread(void)
{
    thread t1(threadfun1);
    thread t2(threadfun2, 10, "abc");
    t1.join();
    cout << "t1 join" << endl;
    t2.detach();
    cout << "t2 detach" << endl;
    return 0;
}

int thread_main(void)
{
    atomic_bool b(0);
    list<thread> lstThread;
    for(int i = 0;i<10;i++)
    {
        lstThread.push_back(thread(threadfun12));
    }
    for(auto & th:lstThread)
    {
        th.join();
    }
    return 0;
}