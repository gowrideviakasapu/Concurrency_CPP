#include<iostream>
#include<thread>
#include<chrono>
using namespace std;

int main(){
    //using lambda function to create task
    thread t1([](){
        for(int i =0; i<10; i++){
            cout<< "Thread1 :" <<i <<endl;
            // You can add time to view the thread execution better
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    });
    thread t2([](){
        for(int i =0; i<10; i++){
            cout<< "Thread2 :" <<i <<endl;
        }
    });
    t1.join();  //both t1, t2 runs simultaneously
    // t1 has wait time 10ms, so the t2 execution is faster
    t2.join();
    return 0;
}