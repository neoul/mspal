#include <iostream>

int main() {
  char32_t *p = L"ABC";
  char16_t *p = u"ABC";
  std::cout << sizeof(long) << std::endl;
  std::cout << sizeof(long long) << std::endl;
  return EXIT_SUCCESS;
}
