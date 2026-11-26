// Write and Read concurrently from Cache
#include <iostream>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <thread>
using namespace std;


template <typename Key, typename Value>
class ConcurrentCache{
    private:
    unordered_map<Key, Value> cache;
    mutable shared_mutex mtx;

    public:
    // get() returns optional Value, so use the optional param for return
    // if Value was not return then nullopt return for std::option
    optional<Value> get(const Key& key){
        shared_lock<shared_mutex> lock(mtx);
        auto it = cache.find(key);
        if (it!=cache.end()){
            return it->second;
        }
        return nullopt;
    }

    void put(const Key& key, const Value& value){
        unique_lock<shared_mutex> lock(mtx);
        cache[key] = value;
    }

    bool remove(Key& key){
        unique_lock<shared_mutex> lock(mtx);
        return cache.erase(key) > 0;
    }

    size_t size(){
        shared_lock<shared_mutex> lock(mtx);
        return cache.size();
    }
};

int main(){
    ConcurrentCache<string, int> mainCache;
    vector<thread> threads;

    // Writing threads
    for(int i=0; i<10; i++){
        threads.emplace_back([& mainCache, i](){
            for(int j =10 ; j<13; j++){
                cout<< "Writing into Cache:" << i << "and" <<j <<endl;
                mainCache.put("key" + to_string(i*j), i*j);
            }
        });
    }

    // Reading threads
    for(int i =0; i<10; i++){
        threads.emplace_back([&mainCache](){
            for(int j=0; j<5; j++){
                auto value = mainCache.get("key"+to_string(j));
                cout << "Reading from Cache:" << j << endl;
            }
        });

    }

    for(auto& t: threads){
        t.join();
    }

    cout << "Cache Size" << mainCache.size() << endl;
    return 0;
}
