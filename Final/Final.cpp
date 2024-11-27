#include <iostream>
#include <fstream>
#include <curl/curl.h>

// Callback function to write data received by cURL to a file
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* outFile = static_cast<std::ofstream*>(userp);
    size_t totalSize = size * nmemb;
    outFile->write(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main() {
    // URL to fetch Tesla stock data in CSV format from Yahoo Finance
    std::string url = "https://query1.finance.yahoo.com/v7/finance/download/TSLA?period1=1672531200&period2=1704067200&interval=1d&events=history&includeAdjustedClose=true";

    // Output file to store data
    std::string outputFileName = "tesla_stock_data.csv";
    std::ofstream outFile(outputFileName, std::ios::out | std::ios::binary);

    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing: " << outputFileName << std::endl;
        return 1;
    }

    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    outFile.close();

    if (res == CURLE_OK) {
        std::cout << "Tesla stock data has been successfully saved to " << outputFileName << std::endl;
    }

    return 0;
}
