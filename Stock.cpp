//
// Created by willi on 5/21/2025.
//

#include "Stock.h"
#include "GetEnv.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <bits/ostream.tcc>
#include <curl/curl.h>
#include <fstream>

using json = nlohmann::json;
Stock::Stock(const std::string& tickerInit) : ticker(tickerInit), fwdPE(0.0), priceInterval(1), period(30) {
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

    std::string fileName = "../price_data_cache/" + ticker + ".json";
    std::filesystem::create_directories("../price_data_cache");


    if (std::filesystem::exists(fileName)) {
        APIResponse response;
        std::ifstream f(fileName.c_str());
        std::cout << "File found" << std::endl;
        std::string line;
        while (std::getline(f, line)) {
            response.data += line;
        }
        f.close();
        parseJsonResponse(response.data);
        return;
    }
    else {
        std::cout << "File not found" << std::endl;
    }

    refreshPriceDataHelper(ticker);
}

void Stock::refreshPriceDataHelper(const std::string& ticker) {
    std::cout << "Refreshing price data for " << ticker << std::endl;

    CURL *curl= curl_easy_init();
    APIResponse response;


    if (curl) {
        const std::string url = buildAPIUrl(ticker);
        std::cout << "URL: " << url << std::endl;
        if (url.empty()) {return;}
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.response_code);

        if (res != CURLE_OK) {
            std::cout << "Error: " << curl_easy_strerror(res) << std::endl;
        }else {
            std::cout << "Response Length: " << response.data << std::endl;
            const bool successParse = parseJsonResponse(response.data);
            std::string fileName = "../price_data_cache/" + ticker + ".json";
            std::ofstream f(fileName.c_str());
            f << response.data;
            f.close();
        }

        curl_easy_cleanup(curl);
    }else {
        std::cout << "Error creating curl" << std::endl;
    }
}

std::string Stock::buildAPIUrl(const std::string& ticker, const std::string& period) const {
    const std::string apiKey = getEnvVar("ALPHA_API_KEY");
    if (apiKey.empty()) {
        std::cout << "API key not found" << std::endl;
        return "";
    }

    std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=";
    url += ticker;
    url += "&interval=5min&outputsize=full&apikey=";

    url += apiKey;
    return url;
}

bool Stock::parseJsonResponse(const std::string& res) {

    json wholeDataJson = json::parse(res);
    json priceDataJson = wholeDataJson["Time Series (5min)"];
    std::vector<PriceData> priceDataTemp;

    for (auto& element : priceDataJson.items()) {
        //std::cout << element.key() << " open: " << element.value()["1. open"] << std::endl;
        priceDataTemp.push_back(PriceData{
            element.key(),
            std::stod(element.value()["1. open"].template get<std::string>()),
            std::stod(element.value()["2. high"].template get<std::string>()),
            std::stod(element.value()["3. low"].template get<std::string>()),
            std::stod(element.value()["4. close"].template get<std::string>()),
            std::stol(element.value()["5. volume"].template get<std::string>())
        });
    }

    setPriceData(priceDataTemp);
    return true;
}

double Stock::getSimpleMovingAverage(int period) {
    double simpleMovingAverage = 0.0;
    for (auto& element : priceData) {
        simpleMovingAverage += element.close;
    }
    return simpleMovingAverage / priceData.size();
}