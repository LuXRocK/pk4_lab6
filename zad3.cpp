#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

std::mutex mutex;
std::condition_variable condition;
bool ready = false;
bool processed = false;
std::atomic<int> j(0);

void add(){
   for(int i = 0; i < 1000000; i++){
    j.fetch_add(1);
   }
}
void sub(){
    std::unique_lock lock(mutex);
    condition.wait(lock, []{ return ready; });

    std::cout << "SUB thread is processing data" << std::endl;

    for(int i = 0; i < 100000; i++){
        j = j - 1;
    }

    std::cout << "SUB thread processed data" << std::endl;

    processed = true;
    lock.unlock();
    condition.notify_one();
}

int main()
{
    std::cout << "j before addition: " << j << std::endl;
    std::thread thread1(add);
    std::thread thread2(add);
    std::thread thread3(sub);
    thread1.join();
    thread2.join();

    
    std::cout << "j after addition: " << j << std::endl;

    {
        std::lock_guard lock(mutex);
        ready = true;
        std::cout << "main() signals j ready for processing" << std::endl;
    }

    condition.notify_one();
 
    {
        std::unique_lock lock(mutex);
        condition.wait(lock, []{ return processed; });
    }

    std::cout << "Back in main(), j = " << j << std::endl;
 
    thread3.join();
}