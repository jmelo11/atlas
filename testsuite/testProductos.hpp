#pragma once
#include "atlas.hpp"

using namespace atlas;

void printProd(CashFlowInstrument& fi) {
	auto d_ = fi.dates();
	auto i_ = fi.interest();
	auto k_ = fi.redemptions();
	for (size_t i = 0; i < k_->size(); i++)
	{
		std::cout << d_->at(i) << ": (I: " << i_->at(i) << ")" << "(K: " << k_->at(i) << ")" << "(P: "<< i_->at(i) + k_->at(i) <<")\n";
	}
}

void testDap(){
	Date start = Date(1, Month::Jan, 2021);
	Date end = Date(1, Month::Aug, 2021);
	double rate_A = 0.03;
	double notional = 100;
	DAP dap_A(start, end, notional, rate_A);
	
	InterestRate rate_B(rate_A, Act360(), Compounding::Simple, Frequency::Annual);
	DAP dap_B(start, end, notional, rate_B);
	
	std::cout << "DAP: \n";
	printProd(dap_B);
	std::cout << "\n";
}
void testBullet() {
	Date start = Date(1, Month::Jan, 2021);
	Date end = Date(1, Month::Jan, 2022);
	double notional = 100;
	InterestRate coupon(0.03, Thirty360(Thirty360::BondBasis), Compounding::Simple, Frequency::Annual);
	FixedBullet bond(start, end, Frequency::Semiannual, notional, coupon);
	
	std::cout << "Bullet: \n";
	printProd(bond);
	std::cout << "\n";
}
void testEP() {
	Date start = Date(1, Month::Jan, 2021);
	Date end = Date(1, Month::Jan, 2022);
	double notional = 100;
	InterestRate coupon(0.03, Thirty360(Thirty360::BondBasis), Compounding::Simple, Frequency::Annual);
	EqualPayment loan(start, end, Frequency::Semiannual, notional, coupon);

	std::cout << "EP: \n";
	printProd(loan);
	std::cout << "\n";
}

/* Ejemplo Multithreading
static std::mutex mu;
static void testPagosIguales(std::vector<EqualPayment>* loans) {
	InterestRate r(0.03, Thirty360(Thirty360::BondBasis), Compounding::Compounded, Frequency::Annual);
	Date start(16, Month::May, 2022);
	Date end(16, Month::May, 2027);
	EqualPayment pi(start, end, Frequency::Monthly, 100, r);

	std::lock_guard<std::mutex> lock(mu);
	loans->push_back(pi);
};

void testMultiT() {
	std::vector<EqualPayment>loans;

	Timer t;
	std::vector<std::future<void>> fs;
	for (size_t i = 0; i < 150000; i++)
	{
		fs.push_back(std::async(std::launch::async, testPagosIguales, &loans));
	}
	for (auto& f : fs)
	{
		f.wait();
	}
}
*/

void testProds() {
	testDap();
	testBullet();
	testEP();
}