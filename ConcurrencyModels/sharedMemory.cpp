#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;


class SharedMemoryCounter {
    private:
        int counter = 0; // shared memory counter
        mutex mtx; // mutex to protect shared memory
    
    public:
        void incrementCounter() {
            lock_guard<mutex> lock(mtx); // lock the mutex to protect shared memory
            counter++;
        }

        int getCounter(){
            lock_guard<mutex> lock(mtx); // lock the mutex to protect shared memory
            cout << "Current Counter Value: " << counter << endl;
            return counter;
        }
};

void multipleThreadsSharedMemory() {
    SharedMemoryCounter sharedCounter;
    vector<thread> threads;

    // creating multiple threads that increment the shared counter
    for(int i = 0; i < 10; i++){
        cout << "Creating thread " << i + 1 << endl;
        threads.emplace_back(&SharedMemoryCounter::incrementCounter, &sharedCounter);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "Final Counter Value: " << sharedCounter.getCounter() << endl;
}

int main() {
    multipleThreadsSharedMemory();
    return 0;
}