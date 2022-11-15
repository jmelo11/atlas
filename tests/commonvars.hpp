/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef EECCE88A_9412_4D10_9BFD_D2D919A4D2A5
#define EECCE88A_9412_4D10_9BFD_D2D919A4D2A5

#include <ql/interestrate.hpp>
#include <ql/quantlib.hpp>
#include <atlas/others/auxfuncs.hpp>
#include <memory>
#include <vector>

using namespace QuantLib;

struct CommonVars {
    Date evalDate         = Date(1, Month::Jan, 2022);
    Date startDate        = Date(1, Month::Jan, 2022);
    Date endDate          = Date(1, Month::Jan, 2025);
    DayCounter dayCounter = Actual360();
    double amount         = 100;
    double spread         = 0;
    double curveRate      = 0.03;
    InterestRate rate     = InterestRate(0.03, dayCounter, Compounding::Simple, Frequency::Annual);
    Frequency paymentFreq = Frequency::Semiannual;
    Schedule schedule     = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFreq);
};

static CommonVars commonVars;

#endif /* EECCE88A_9412_4D10_9BFD_D2D919A4D2A5 */
