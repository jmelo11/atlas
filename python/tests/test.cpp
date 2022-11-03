#include "pch.hpp"
#include <atlaspython.hpp>
#include <fstream>
#include <iostream>

using namespace atlaspythonwrapper;

json readJSONFile(std::string filePath) {
    std::ifstream file(filePath);
    std::ostringstream tmp;
    tmp << file.rdbuf();
    std::string s = tmp.str();
    json data     = json::parse(s);
    return data;
}

TEST(TestParRateEqualPayment, TestPython) {
    json data   = readJSONFile("json/equalpayment.json");
    double rate = pricer<EqualPayment, ParRateSolver>::defineAndCalculate(data);
    EXPECT_EQ(true, true);
}
TEST(TestParRateFixedBullet, TestPython) {
    json data   = readJSONFile("json/equalpayment.json");
    double rate = pricer<FixedBullet, ParRateSolver>::defineAndCalculate(data);
    EXPECT_EQ(true, true);
}
TEST(TestParSpreadFloatingBullet, TestPython) {
    json data     = readJSONFile("json/floatingbullet.json");
    double spread = pricer<FloatingBullet, ParSpreadSolver>::defineAndCalculate(data);
    EXPECT_EQ(true, true);
}