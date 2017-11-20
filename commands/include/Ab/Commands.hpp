
#if !defined(AB_COMMANDS_HPP_)
#define AB_COMMANDS_HPP_

namespace Ab {

int cmd_asm(int argc, char** argv);

int cmd_disasm(int argc, char** argv);

int cmd_run(int argc, char** argv);

int cmd_help(int argc, char** argv);

int cmd_version(int argc, char** argv);

extern const char* const SHORT_HELP_STRING;

extern const char* const HELP_STRING;

extern const char* const USAGE_STRING;

}  // namespace Ab

#endif // AB_COMMANDS_HPP_
