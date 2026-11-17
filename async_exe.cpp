#include <iostream>
#include <chrono>
#include <future>
#include <thread>
using namespace std;

int returnRandomNumber(){
    return rand()%100;
}

void otherWork(){
    cout << "Doing other work..." << endl;
    for(int i = 5; i > 0; i--){
        cout << "Working... : " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

    
void asyncExecution(){
    // Launching a task asynchronously
    cout << "Launching async task..." << endl;
    auto future = async(launch::async, returnRandomNumber);

    //do some other work while the task is running
    otherWork();

    cout << "Result - Random Number: " << future.get() << endl; // Waits for the result

    // try to get results multiple times
    // and catch the exception if the future is already retrieved
    try{
        cout << "Trying to get the result again..." << future.get() << endl;
    }
    catch(const std::future_error& e)
    {
        cout << "Caught future_error: " << e.what() << endl;
    }
}

int main(){

    asyncExecution();
    return 0;
}