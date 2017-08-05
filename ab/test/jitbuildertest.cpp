#include <Jit.hpp>
#include <ab.hpp>
#include <cstdint>
#include <gtest/gtest.h>
#include <ilgen/MethodBuilder.hpp>
#include <ilgen/TypeDictionary.hpp>

namespace ab {

class SimpleMethod : public TR::MethodBuilder {
public:
	SimpleMethod(TR::TypeDictionary* types);
	virtual bool buildIL() override;
};

SimpleMethod::SimpleMethod(TR::TypeDictionary* types) : TR::MethodBuilder(types) {
	DefineFile(__FILE__);
	DefineLine(LINETOSTR(__LINE__));

	// int32_t increment(int32_t value);
	DefineName("increment");
	DefineParameter("value", Int32);
	DefineReturnType(Int32);
}

bool SimpleMethod::buildIL() {
	// return (value + 1);
	Return(Add(Load("value"), ConstInt32(1)));
	return true;
}

TEST(JitBuilderTest, Example) {
	initializeJit();

	TR::TypeDictionary types;
	SimpleMethod method(&types);
	uint8_t* entry = 0;
	ASSERT_EQ(compileMethodBuilder(&method, &entry), 0);

	auto increment = (int32_t(*)(int32_t))(entry);
	for (int32_t x : {1, 2, 10, -15}) {
		EXPECT_EQ(increment(x), x + 1);
	}

	shutdownJit();
}

} // namespace ab