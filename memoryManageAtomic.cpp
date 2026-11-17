#include <iostream>
#include <thread>
#include <atomic>
using namespace std;


// In multithreaded operation, counter++ is actually split into three CPU operations: 
// read, modify, and write.
// If two threads perform counter++ at the same time, they may interleave causing the updates overwrite each other,
//std::atomic template treats this problem by ensuring that the read-modify-write operation without interruption.


int main(){
    std::atomic<int> safeCounter{0}; // atomic counter
    thread t1([&safeCounter](){ 
        for(int i = 0; i < 10; i++){
            safeCounter.fetch_add(1);
            cout << "Thread 1: Counter Value: " << safeCounter << endl;
        }
    });

    thread t2([&safeCounter](){
        for(int i = 0; i < 5; i++){
            safeCounter.fetch_add(1);
            cout << "Thread 2: Counter Value: " << safeCounter << endl;
        }
    });
    t1.join();
    t2.join();

    cout << "Final Counter Value: " << safeCounter << endl;
    return 0;
}