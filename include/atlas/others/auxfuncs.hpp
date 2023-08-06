/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef A1E6F5B0_6F4A_4460_9F42_A14B525D8447
#define A1E6F5B0_6F4A_4460_9F42_A14B525D8447

#include <chrono>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

namespace Atlas {

    /**
     * @class Timer
     * @brief Timer class
     *
     */
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
    inline std::vector<std::vector<T>> sliceVector(const std::vector<T>& input, size_t n) {
        std::vector<std::vector<T>> r;
        size_t rSize = input.size();
        size_t i;
        for (i = 0; i < rSize - n; i = i + n) {
            std::vector<T> tmp(input.begin() + i, input.begin() + i + n);
            r.push_back(tmp);
        }
        std::vector<T> tmp(input.begin() + i, input.end());
        r.push_back(tmp);
        return r;
    }

    inline Date fastDateParser(const std::string& date) {
        int day   = std::stoi(date.substr(0, 2));
        int month = std::stoi(date.substr(2, 2));
        int year  = std::stoi(date.substr(4, 4));
        return Date(day, (Month)month, year);
    }

    inline std::vector<std::tuple<Date, Date, double>> calculateOutstanding(const std::map<Date, double>& disbursements,
                                                                            const std::map<Date, double>& redemptions) {
        std::vector<std::tuple<Date, Date, double>> outstanding;

        // Copy disbursements to a mutable map
        std::map<Date, double> remainingDisbursements = disbursements;

        // For each redemption
        for (const auto& redemptionPair : redemptions) {
            Date redemptionDate     = redemptionPair.first;
            double redemptionAmount = redemptionPair.second;

            // Find a disbursement to apply the redemption to
            for (auto& disbursementPair : remainingDisbursements) {
                if (disbursementPair.second > 0) {
                    Date disbursementDate      = disbursementPair.first;
                    double& disbursementAmount = disbursementPair.second;

                    // Calculate outstanding amount for the period
                    double periodOutstanding = std::min(disbursementAmount, redemptionAmount);

                    // Add outstanding amount for the period to the result
                    outstanding.push_back(std::make_tuple(disbursementDate, redemptionDate, periodOutstanding));

                    // Update disbursement and redemption amounts
                    disbursementAmount -= periodOutstanding;
                    redemptionAmount -= periodOutstanding;

                    if (redemptionAmount <= 0) {
                        break;  // Break the loop if the redemption is fully applied
                    }
                }
            }
        }

        return outstanding;
    }

    inline void printTable(const std::vector<std::vector<std::string>>& tableData) {
        // Find the maximum width of each column
        std::vector<size_t> columnWidths;
        for (const auto& row : tableData) {
            if (columnWidths.size() < row.size()) columnWidths.resize(row.size(), 0);

            for (size_t i = 0; i < row.size(); ++i) {
                if (row[i].length() > columnWidths[i]) { columnWidths[i] = row[i].length(); }
            }
        }

        // Print the table
        for (const auto& row : tableData) {
            for (size_t i = 0; i < row.size(); ++i) {
                std::cout << std::left << std::setw(columnWidths[i]) << row[i];
                if (i < row.size() - 1) { std::cout << " | "; }
            }
            std::cout << std::endl;
        }
    }
}  // namespace Atlas

#endif /* A1E6F5B0_6F4A_4460_9F42_A14B525D8447 */
