#include <iostream>
#include <thread>
#include <future>
using namespace std;

void foo(promise<int> p){
    p.set_value(10);
}

int main(){
    //creating a future object and a thread that executes the function foo
    // which sets the value of the promise asynchronously.
    // The future object is used to retrieve the value set by the promise.
    promise<int> p;
    future<int> f = p.get_future();
    //std::move is used to transfer ownership of the promise to the thread
    // instead of copying it, since promise is not copyable.
    thread t(foo, move(p));
    t.join();
    cout << "Result: " << f.get() << endl;
    return 0;
}
