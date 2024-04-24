#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::vector<int> vec;
std::mutex mutex;

void adder() {
    for(int i=0; i<1000000; i++){
        std::lock_guard guard(mutex);
        vec.push_back(i);
    }
}

void sub(){
    for(int i=0; i<500000; i++){
        std::lock_guard guard(mutex);
        vec.pop_back();
    }
}

int main() {
  std::thread adder1(adder);
  std::thread adder2(adder);

  adder1.join();
  adder2.join();

  std::thread sub1(sub);
  std::thread sub2(sub);

  sub1.join();
  sub2.join();

  std::cout << vec.size() << std::endl;
  return 0;
}