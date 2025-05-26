//
// Created by willi on 5/21/2025.
//

#include "Stock.h"

#include <iostream>
#include <bits/ostream.tcc>
#include <curl/curl.h>


Stock::Stock(std::string ticker) {
    ticker = ticker;
    std::cout << "Stock created " << ticker << std::endl;

    CURL *curl = curl_easy_init();
    if (curl) {

    }
}
