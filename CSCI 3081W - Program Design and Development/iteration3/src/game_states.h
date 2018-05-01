/**
 * @file game_states.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */

#ifndef SRC_GAME_STATES_H_
#define SRC_GAME_STATES_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/
/**
 * @brief The states the game can be in.
 */
enum game_states {
  kRunning,
  kPaused,
  kWon,
  kLost
};
NAMESPACE_END(csci3081);

#endif  // SRC_GAME_STATES_H_
