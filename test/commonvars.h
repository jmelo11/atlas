#pragma once
#include <atlas.hpp>

struct CommonVars {
	atlas::Date evalDate = atlas::Date(1, atlas::Month::Jan, 2022);
	atlas::Date startDate = atlas::Date(1, atlas::Month::Jan, 2022);
	atlas::Date endDate = atlas::Date(1, atlas::Month::Jan, 2025);
	atlas::DayCounter dayCounter = atlas::Act360();
	double amount = 100;
	double spread = 0;
	double curveRate = 0.03;	
	atlas::InterestRate rate = atlas::InterestRate(0.03, dayCounter, atlas::Compounding::Simple, atlas::Frequency::Annual);
	atlas::Frequency paymentFreq = atlas::Frequency::Semiannual;
	atlas::Schedule schedule = atlas::makeSchedule().from(startDate).to(endDate).withFrequency(paymentFreq);
	atlas::MetaData metaData = atlas::MetaData();
};
	
static CommonVars commonVars;
