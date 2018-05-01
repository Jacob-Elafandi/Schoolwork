/**
 * @file config_file_parser.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_CONFIG_FILE_PARSER_H_
#define SRC_CONFIG_FILE_PARSER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class called from main that parses an input file.
 */
class ConfigFileParser {
 public:
  /**
   * @brief Initialize an object that parses an input file.
   */
  ConfigFileParser(): aparams_() {}

  /**
   * @brief Parse an input file.
   */
  arena_params Parse(int argc, char ** argv);

 private:
  arena_params aparams_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_CONFIG_FILE_PARSER_H_
