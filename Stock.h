//
// Created by william on 5/21/2025.
//

#ifndef STOCK_H
#define STOCK_H

#include <iostream>
#include <string>
#include <vector>
#include <bits/ostream.tcc>

struct PriceData {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    long volume;
};

struct APIResponse {
    std::string data;
    long response_code;
};

class Stock {
private:
    std::string ticker;
    std::vector<PriceData> priceData;
    double fwdPE;
    int priceInterval;
    int period;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, APIResponse* response);
    std::string buildAPIUrl(const std::string& ticker, const std::string& period = "1mo") const;
    bool parseJsonResponse(const std::string& res);

    inline void setPriceData(const std::vector<PriceData>& priceDataInit) {
        priceData = priceDataInit;
        std::cout << "Price data length" <<priceData.size() << std::endl;
    };
public:
    Stock(const std::string& tickerInit);
    ~Stock();

    void refreshPriceData();
    void refreshPriceDataHelper(const std::string& ticker);

    double getSimpleMovingAverage( int period = 0);

};



#endif //STOCK_H
