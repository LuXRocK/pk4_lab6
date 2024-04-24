#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::vector<int> vec;
std::mutex mutex;
int out = 1;

void producer_() {
    while(true){
        std::string x;
        std::cin >> x;
        if(x == "KONIEC"){
            break;
        }
        std::lock_guard guard(mutex);
        vec.push_back(std::stoi(x));
        std::this_thread::yield();
    }
}

void consument_(){
    while(true){
        std::lock_guard guard(mutex);
        if(vec.empty()){
            std::this_thread::yield();
            continue;
        }
        out *= vec.back();
        vec.pop_back();
    }
}

int main() {
  std::thread producer(producer_);
  std::thread consument(consument_);

  producer.join();
  consument.detach();

  std::cout << "WYNIK: " << out << std::endl;
  return 0;
}