#include <Ab/Test/Env.hpp>

#include <gtest/gtest.h>

extern "C" {

int main(int argc, char* argv[]) {
	Ab::Test::env = new Ab::Test::Env();
	testing::AddGlobalTestEnvironment(Ab::Test::env);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

}  // extern "C"
