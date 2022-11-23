#include "pch.hpp"
#include <makeobjectfromjson.hpp>
#include <schemas/depositschema.hpp>

using namespace Atlas;

TEST(MakeObj, Deposit) {
    json data = R"({
        "STARTDATE": "01012022",
        "ENDDATE": "01012023",
        "NOTIONAL": 100,
        "RATE": {
            "VALUE": 0.01,
            "DAYCOUNTER": "ACT360",
            "COMPOUNDING": "SIMPLE",
            "FREQUENCY": "ANNUAL"
        }
    })"_json;

    EXPECT_NO_THROW(makeObjectFromJson<Deposit>(data));
}
