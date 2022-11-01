#ifndef CB297169_59F8_4774_A391_C6A6C58B0BBA
#define CB297169_59F8_4774_A391_C6A6C58B0BBA

#include <qlp/schemas/commonschemas.hpp>
#include <qlp/schemas/schema.hpp>
#include <atlas/instruments/fixedrateinstruments.hpp>
#include <atlas/instruments/floatingrateinstruments.hpp>

namespace AtlasPython {
    using namespace Atlas;
    
    //FixedBullet
    template <>
    void QuantLibParser::Schema<FixedBullet>::initSchema();

    template <>
    void QuantLibParser::Schema<FixedBullet>::initDefaultValues();

    //DAP
    template <>
    void QuantLibParser::Schema<DAP>::initSchema();

    template <>
    void QuantLibParser::Schema<DAP>::initDefaultValues();

    //FixedCustom
    template <>
    void QuantLibParser::Schema<FixedCustomStructure>::initSchema();

    template <>
    void QuantLibParser::Schema<FixedCustomStructure>::initDefaultValues();

    //EqualPayment
    template <>
    void QuantLibParser::Schema<EqualPayment>::initSchema();

    template <>
    void QuantLibParser::Schema<EqualPayment>::initDefaultValues();

}  // namespace AtlasPython

#endif /* CB297169_59F8_4774_A391_C6A6C58B0BBA */
