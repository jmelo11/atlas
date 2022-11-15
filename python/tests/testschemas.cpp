#include "pch.hpp"
#include <schemas/dapschema.hpp>

using namespace QuantLibParser;

TEST(DAPSchema, TestSchema) {
    json instrument = R"({})"_json;
    Schema<DAP> schema;
    EXPECT_NO_THROW(schemaValidator.validate(instrument));
}