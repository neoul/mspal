#include "hello-world.h"

std::string say_hello() {
  SYSTEMTIME TheTime;
  GetSystemTime(&TheTime);
  return std::string("Hello, CMake world!") + std::to_string(TheTime.wYear);
}

int main() {
  std::cout << say_hello() << std::endl;
  return EXIT_SUCCESS;
}