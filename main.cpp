#include <iostream>

#include "Stock.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Stock apple("LUNR");

    std::cout << "Moving Average: " << apple.getSimpleMovingAverage() << std::endl;

    return 0;
}
