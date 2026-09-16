#include <cstdlib>
#include "SharedPtr.h"

int main() {
    SharedPtr<int> ptr1(new int(7));
   return EXIT_SUCCESS;
}
