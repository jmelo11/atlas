/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef A1E6F5B0_6F4A_4460_9F42_A14B525D8447
#define A1E6F5B0_6F4A_4460_9F42_A14B525D8447

#include <chrono>
#include <iostream>
#include <vector>

namespace Atlas {

    class Timer {
       public:
        Timer() { startPoint = std::chrono::high_resolution_clock::now(); };
        ~Timer() {
            auto endPoint = std::chrono::high_resolution_clock::now();
            auto start    = std::chrono::time_point_cast<std::chrono::milliseconds>(startPoint).time_since_epoch().count();
            auto end      = std::chrono::time_point_cast<std::chrono::milliseconds>(endPoint).time_since_epoch().count();
            std::cout << "Elapsed time: " << (end - start) << "ms \n";
        };

       private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
    };

    template <class T>
    static std::vector<std::vector<T>> sliceVector(const std::vector<T>& input, size_t n) {
        std::vector<std::vector<T>> r;
        size_t rSize = input.size();
        size_t step  = rSize / n;
        size_t i;
        for (i = 0; i < rSize - n; i = i + n) {
            std::vector<T> tmp(input.begin() + i, input.begin() + i + n);
            r.push_back(tmp);
        }
        std::vector<T> tmp(input.begin() + i, input.end());
        r.push_back(tmp);
        return r;
    }

    static Date fastDateParser(const std::string& date) {
        int day   = std::stoi(date.substr(0, 2));
        int month = std::stoi(date.substr(2, 2));
        int year  = std::stoi(date.substr(4, 4));
        return Date(day, (Month)month, year);
    }
}  // namespace Atlas

#endif /* A1E6F5B0_6F4A_4460_9F42_A14B525D8447 */
