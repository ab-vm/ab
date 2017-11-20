#if !defined(AB_TESTUTIL_ENV_HPP_)
#define AB_TESTUTIL_ENV_HPP_

#include <Ab/CppUtilities.hpp>

#include <Ab/Test/Config.hpp>

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

namespace Ab::Test {

inline std::string read_file(const std::string& filename) {
	std::ifstream in(filename, std::ios::in);
	if (!in.is_open()) {
		throw std::runtime_error(std::string("failed to open input file: ") + filename);
	}
	using iter = std::istreambuf_iterator<char>;
	return std::string(iter(in), iter());
}

inline std::string env_or(const char* var, std::string_view fallback) {
	const char* value = std::getenv(var);
	if (value != nullptr) {
		return std::string(value);
	}
	return std::string(fallback);
}

#define AB_TEST_GET_CONFIG(name) env_or(AB_STRINGIFY_RAW(name), name)

class Env : public ::testing::Environment {
public:
	Env() = default;

	Env(Env&&) = delete;

	Env(const Env&) = delete;

	virtual ~Env() override = default;

	virtual void SetUp() override { source_dir_ = AB_TEST_GET_CONFIG(AB_SOURCE_DIR); }

	virtual void TearDown() override {}

	const std::string& source_dir() const noexcept { return source_dir_; }

	std::string source_filename(const std::string& filename) const noexcept {
		return source_dir() + "/" + filename;
	}

	std::string read_source_file(const std::string& filename) const {
		return read_file(source_filename(filename));
	}

private:
	std::string source_dir_;
};

extern Env* env;

}  // namespace Ab::Test

#endif  // AB_TESTUTIL_ENV_HPP_
