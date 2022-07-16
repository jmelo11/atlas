#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include "QL.hpp"

namespace atlas {
	class Timer
	{
	public:
		Timer() {
			startPoint = std::chrono::high_resolution_clock::now();
		};
		~Timer() {
			auto endPoint = std::chrono::high_resolution_clock::now();
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();
			std::cout << "Elapsed time: " << (end-start) << "us \n";
		};

	private:
		std::chrono::time_point< std::chrono::high_resolution_clock> startPoint;
	};

	template <class T>
	std::vector<std::vector<T>> sliceVector(const std::vector<T>& input, size_t n) {
		std::vector<std::vector<T>> r;
		size_t rSize = input.size();
		size_t step = rSize / n;
		size_t i;
		for (i = 0; i < rSize - n; i = i + n)
		{
			std::vector<T> tmp(input.begin() + i, input.begin() + i + n);
			r.push_back(tmp);
		}
		std::vector<T> tmp(input.begin() + i, input.end());
		r.push_back(tmp);
		return r;
	}	

	Date fastDateParser(const std::string& date) {
		int day = std::stoi(date.substr(0, 2));
		int month = std::stoi(date.substr(2, 2));
		int year = std::stoi(date.substr(4, 4));
		return Date(day, (Month)month, year);
	}
}