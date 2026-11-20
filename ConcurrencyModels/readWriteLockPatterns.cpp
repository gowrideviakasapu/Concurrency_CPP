// Multiple readers and Writer needs exclusive write privilage
// shared mutex allows multiple threads to read shared data at the same time,
// while allowing only one thread can write it
#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
using namespace std;


class ReadWrite{
    private:
    int data = 0;
    mutable shared_mutex sMtx;

    public:
    int readData() const{
        // shared_lock for multiple reads
        shared_lock<shared_mutex> lock(sMtx);
        return data;
    }
    void writeData(int value){
        //unique_lock for exclusive write
        unique_lock<shared_mutex> lock(sMtx);
        data = value;
    }

};

int main(){
    ReadWrite rW;
    vector<thread> threads;

    // single write
    rW.writeData(2000);
    rW.writeData(1999);

    // initial read threads reading 1999, once write updated for every 50sec,
    // read threads reading 0,1,2..
    //multiple read
    for(int i =0; i<5; i++){
        threads.emplace_back([&rW, i]{
            for(int j =0; j<10; j++){
                int readValue = rW.readData();
                cout << "Reader:" <<i << ",Read Value:" << readValue <<endl;
                this_thread::sleep_for(chrono::milliseconds(10));
            }
        });
    }

    //single write access using threads
    threads.emplace_back([&rW]{
        for (int i =0; i<5; i++){
            rW.writeData(i);
            cout << "Write Value:" << i << endl;
            this_thread::sleep_for(chrono::milliseconds(50));
        }
    });

    for(auto& t : threads){
        t.join();
    }

    return 0;
}
