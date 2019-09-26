#include <Ab/Config.hpp>
#include <Ab/Assert.hpp>
#include <Ab/Commands.hpp>
#include <Ab/Version.hpp>

#include <fmt/format.h>
#include <memory>
#include <optional>
#include <string_view>
#include <unistd.h>
#include <utility>
#include <vector>

#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Ab;

const char* bin_name = nullptr;
const char* cmd_name = nullptr;

void print_usage() {
	fmt::print(
		"Usage: {} [<option>...] [--] <command>\n"
		"See `ab help`.\n",
		bin_name);
}

void print_version() {
	fmt::print("{}: {}\n", bin_name, Version::STRING);
}

/// Helper for allocating directory-entry structs.
/// The name_max depends on the underlying filesystem, which is determined
/// from the directory path.
///
dirent* alloc_dirent(const char* dirname) {
	auto name_max = pathconf(dirname, _PC_NAME_MAX);
	AB_ASSERT(name_max != -1);
	auto size = offsetof(struct dirent, d_name) + name_max + 1;
	return reinterpret_cast<dirent*>(malloc(size));
}

/// Find file by basename, in directory. Will not recurse. Returns nullopt if
/// the file doesn't exist
///
std::optional<std::string>
find_file_in_dir(const std::string& filename, const std::string& dirname) {
	std::optional<std::string> result(std::nullopt);
	auto dir = opendir(dirname.c_str());
	if (dir != nullptr) {
		std::unique_ptr<dirent> entry_buffer(alloc_dirent(dirname.c_str()));

		while (true) {
			dirent* entry;
			auto error = readdir_r(dir, entry_buffer.get(), &entry);
			if (error != 0 || entry == nullptr) {
				break;
			}
			if (filename == entry->d_name) {
				result.emplace(dirname + "/" + filename);
				break;
			}
		}

		closedir(dir);
	}
	return result;
}

/// find the seperator, or the end of the string.
std::string_view::size_type find_sep(std::string_view str, char sep, std::size_t pos = 0) {
	auto i = str.find(sep, pos);
	if (i == std::string_view::npos) {
		return str.length();
	}
	return i;
}

std::optional<std::string>
find_file_in_search_path(const std::string& name, const std::string& path) {
	if (path.length() == 0) {
		return std::nullopt;
	}

	std::string_view::size_type a = 0;
	std::string_view::size_type b = 0;

	while (b < path.length()) {
		b = find_sep(path, ':', a);

		auto dir  = std::string(path.substr(a, b - a));
		auto path = find_file_in_dir(name, dir);

		if (path) {
			return path;
		}

		a = b + 1;
	}

	return std::nullopt;
}

std::optional<std::string> find_file_in_env_search_path(const std::string& name, const char* var) {
	const char* path = getenv(var);
	if (path != nullptr) {
		return find_file_in_search_path(name, path);
	}
	return std::nullopt;
}

/// mangle name with ab- prefix.
///
std::string cmd_exe_name(std::string target) { return std::string("ab-") + target; }

std::optional<std::string> find_cmd(const std::string& cmd) {
	std::string name = cmd_exe_name(cmd);
	std::optional<std::string> exe = std::nullopt;

	exe = find_file_in_env_search_path(name, "AB_PATH");
	if (exe) {
		return exe;
	}

	auto ab_home = getenv("AB_HOME");
	if (ab_home) {
		exe = find_file_in_dir(name, std::string(ab_home) + "/bin");
		if (exe) {
			return exe;
		}
	}

	exe = find_file_in_env_search_path(name, "PATH");
	if (exe) {
		return exe;
	}

	return std::nullopt;
}

int exec_cmd(const std::string& cmd_bin, [[maybe_unused]] int argc, char* argv[]) {
	return execv(cmd_bin.c_str(), argv);
}

[[noreturn]] void run_cmd(const char* cmd_name, int argc, char* argv[]) {
	std::optional<std::string> search = find_cmd(cmd_name);
	if (!search) {
		fmt::print(
			"Error: '{}' is not an ab command.\n"
			"See 'ab help'.\n",
			cmd_name);
		exit(1);
	}
	const auto& cmd_bin = *search;
	exec_cmd(cmd_bin, argc, argv);
	fmt::print("Error: failed to exec command binary: {}\n", cmd_bin);
	exit(1);
}

extern "C" int main(int argc, char* argv[]) {
	// process_args(argc, argv);

	bin_name = argv[0];

	int i = 1;
	while (i < argc) {
		const char* arg = argv[i];

		if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
			print_usage();
			exit(0);
		} else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
			print_version();
			exit(0);
		} else if (strcmp(arg, "--") == 0) {
			++i;
			break;
		} else if (arg[0] == '-') {
			fmt::print("Error: unrecognized option '{}'\n", arg);
			exit(1);
		} else {
			break;
		}
	}

	if (argc - i < 1) {
		fmt::print("Error: missing command\n");
		print_usage();
		return 1;
	}

	cmd_name = argv[i];
	run_cmd(cmd_name, argc - 1, argv + 1);
	return 0;
}
