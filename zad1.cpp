#include <iostream>
#include <thread>
#include <mutex>

void printMessage() {
  int i=0;

  while(true){
    std::cout << i++ << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

int main() {
  std::thread thread1(printMessage);

  thread1.join();
  return 0;
}