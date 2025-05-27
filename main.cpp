#include <iostream>

#include "Stock.h"
#include <fstream>
#include <filesystem>
int main() {
    std::cout << "Hello, World!" << std::endl;

    Stock lunr("LUNR");

    std::cout << "Moving Average: " << lunr.getSimpleMovingAverage() << std::endl;

    return 0;
}
