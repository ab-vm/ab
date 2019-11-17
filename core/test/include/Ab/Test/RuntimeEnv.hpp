#ifndef AB_TEST_RUNTIMEENV_HPP_
#define AB_TEST_RUNTIMEENV_HPP_

#include <Ab/Config.hpp>
#include <Ab/Runtime.hpp>
#include <gtest/gtest.h>

namespace Ab::Test {

/// A testing environment
///
class RuntimeEnv : public ::testing::Environment {
public:
	RuntimeEnv() = default;

	RuntimeEnv(RuntimeEnv&&) = delete;

	RuntimeEnv(const RuntimeEnv&) = delete;

	virtual ~RuntimeEnv() override = default;

	virtual void SetUp() override {}

	virtual void TearDown() override {}

	Runtime* get() { return &runtime_; }

private:
	Runtime runtime_;
};

extern RuntimeEnv* runtime_env;

inline Runtime* runtime() { return runtime_env->get(); }

}  // namespace Ab::Test

#endif  // AB_TEST_RUNTIMEENV_HPP_
