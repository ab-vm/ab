#include <Ab/Test/Env.hpp>
#include <Ab/Test/RuntimeEnv.hpp>

#include <gtest/gtest.h>

extern "C" {

int main(int argc, char* argv[]) {
	Ab::Test::env = new Ab::Test::Env();
	testing::AddGlobalTestEnvironment(Ab::Test::env);
	Ab::Test::runtime_env = new Ab::Test::RuntimeEnv();
	testing::AddGlobalTestEnvironment(Ab::Test::runtime_env);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

}  // extern "C"
