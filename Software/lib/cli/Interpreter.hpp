/**
 * @file Interpreter.hpp
 * @author Commentsareuseless
 * @brief Simple command line interpretter for debugging
 * @version 0.1
 * @date 2023-04-22
 */

#include <array>
#include <cstdint>
#include <string_view>

namespace lib
{
namespace cli
{

/**
 * @brief This is common type for callback function's IDs
 */
typedef uint32_t CommandID_t;

/**
 * @brief All commands follow this signature.
 *        Convention is, that CLI interpretter passes
 *        arguments to callback as string, and callback
 *        returns UNIX-like errors where:
 *        0 - No error
 *        anything < 0 - An error occured and error code is returned
 */
using CommandCallback = int (*)(const char*);

using ConsoleHandler = void (*)(const char);

constexpr uint32_t CMD_NAME_MAX_SIZE{20};
constexpr uint32_t CMD_NAME_MIN_SIZE{2};

/**
 * @brief A container to hold callback to command and their
 *        unique identyfication number
 *
 */
struct Command {
  CommandID_t id;
  CommandCallback cmd;

  constexpr Command(const char* name, CommandCallback cmdCallback) noexcept :
      id{IdFromStringName(name)}, cmd{cmdCallback} {}

  constexpr static CommandID_t IdFromStringName(const char* name) noexcept {
    constexpr uint32_t randomizerNum = 0xa5936adbu;
    const uint32_t nameLen{constStrlen(name)};
    CommandID_t result = 0u;

    for (uint32_t i{0}; i < nameLen; ++i) {
      result += static_cast<uint32_t>(name[i]) * i;
    }

    return result ^ randomizerNum;
  }

private:
  constexpr static uint32_t constStrlen(const char* str) {
    constexpr uint32_t maxStrLen = CMD_NAME_MAX_SIZE;
    uint32_t strIter = 0u;

    while ((str[strIter] != '\0') && (maxStrLen > strIter)) { ++strIter; }

    return strIter;
  }
};

/**
 * @brief Call command by supplying their name and optionally
 *        additional arguments
 *
 * @param cmdName - Unique command name
 * @param cmdArgs - Arguments passed to command function
 * @return Execution status
 */
int Call(const char* cmdName, const char* cmdArgs);

/**
 * @brief Function tokenizes
 *
 * @param input
 */
void HandleCliInput(const char* input);

/**
 * @brief Registers putc-like function which handles
 *        printing singular characters to serial console
 *
 * @param callback - callback to putc function
 */
constexpr void RegisterStdoutHandler(ConsoleHandler callback);

} // namespace cli
} // namespace lib
