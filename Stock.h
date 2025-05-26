//
// Created by william on 5/21/2025.
//

#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <vector>

struct PriceData {
    double open;
    double high;
    double low;
    double close;

};

class Stock {
private:
    std::string ticker;
    std::vector<PriceData> priceData;
    double fwdPE;
    int priceInterval;
    int window;
public:
    Stock(std::string ticker);
    void refreshPriceData(std::string ticker);


};



#endif //STOCK_H
