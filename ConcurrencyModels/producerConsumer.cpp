// COmmunication between the threads using the producer-consumer model
// Queue is used to store the data produced by the producer thread
// and consumed by the consumer thread
#include <iostream>
#include <chrono>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
using namespace std;


template<typename T>
class ThreadSafeQueue{
    private:
    // Queue to store the message
    queue<T> messageQueue;
    mutex mtx;
    condition_variable flag; // to set the flag when new data is available

    public:
    void push(T& value){
        //lock guard locks the mutex and unlock when the object goes out of scope
        {
        lock_guard<mutex> lock(mtx);
        // push() - insert items from the back of the queue
        messageQueue.push(value);
        }  // use inside the braces so you dont have to manually unlock
        // notify the condition variable that new data is available in queue
        flag.notify_one();
    }
    T pop(){
        // unique lock is used to conditionally lock a mutex
        {
        unique_lock<mutex> lock(mtx);
        // wait for the condition to set: new data available in the queue
        flag.wait(lock, [this]{return !messageQueue.empty();});
        }
        // front() is used to access the items from the front
        // back() for access from the back
        T value = messageQueue.front();
        // pop() removes the front element
        messageQueue.pop();
        return value;
    }

};

void producerConsumerThreads(){
    ThreadSafeQueue<int> queue;

    thread producer([&queue](){
        for(int i=0; i<10; i++){
            queue.push(i);
            // wait for the message to be pushed into queue and notify the condition variable
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    });

    thread consumer([&queue](){
        for(int i =0; i<10; i++){
            int value = queue.pop();
            cout << "Received value in Queue:" << value << endl;
        }
    });

    // if you forgot to join:
    // terminate called without an active exception
    producer.join();
    consumer.join();
}

int main(){
    cout << "Message queue execution" <<endl;
    // Producer fills the queue and consumer consumes the values from the queue.
    producerConsumerThreads();
    return 0;
}