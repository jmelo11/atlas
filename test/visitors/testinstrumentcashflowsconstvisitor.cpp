#ifndef C8B4E847_D420_42A8_A7AC_A1BDBC8381CF
#define C8B4E847_D420_42A8_A7AC_A1BDBC8381CF

#include "../testsetup.hpp"
#include <atlas/visitors/cashflowaggregation/instrumentcashflowsconstvisitor.hpp>

using namespace Atlas;

TEST(InstrumentCashflowsConstVisitor, FixedRateInstrument) {
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;
    InstrumentCashflowsConstVisitor cashflowsVisitor;
    cashflowsVisitor(instrument);
}

#endif /* C8B4E847_D420_42A8_A7AC_A1BDBC8381CF */
