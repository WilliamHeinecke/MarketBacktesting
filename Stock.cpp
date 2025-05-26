//
// Created by willi on 5/21/2025.
//

#include "Stock.h"

#include <iostream>
#include <bits/ostream.tcc>
#include <curl/curl.h>


Stock::Stock(const std::string& tickerInit) : ticker(tickerInit), fwdPE(0.0), priceInterval(1), window(30) {
    std::cout << "Stock created " << ticker << std::endl;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    refreshPriceData();
}

Stock::~Stock() {
    curl_global_cleanup();
}

size_t Stock::WriteCallback(void* contents, size_t size, size_t nmemb, APIResponse* response) {
    size_t totalSize = size * nmemb;
    response->data.append((char*)contents, totalSize);
    return totalSize;
}

void Stock::refreshPriceData() {

    refreshPriceDataHelper(ticker);
}

void Stock::refreshPriceDataHelper(const std::string& ticker) {
    std::cout << "Refreshing price data for " << ticker << std::endl;

    CURL *curl= curl_easy_init();
    APIResponse response;

    if (curl) {
        const std::string url = buildAPIUrl(ticker);
        std::cout << "URL: " << url << std::endl;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.response_code);

        if (res != CURLE_OK) {
            std::cout << "Error: " << curl_easy_strerror(res) << std::endl;
        }else {
            std::cout << "Response: " << response.data.length() << std::endl;
        }

        curl_easy_cleanup(curl);
    }else {
        std::cout << "Error creating curl" << std::endl;
    }
}

std::string Stock::buildAPIUrl(const std::string& ticker, const std::string& period) const {
    std::string url = "https://query1.finance.yahoo.com/v7/finance/download/";
    url += ticker;
    url += "?period1=1588278400&period2=1650860800&interval=1d&events=history&includeAdjustedClose=true";
    return url;
}

bool Stock::parseResponse(const std::string& res) {

    return false;
}
