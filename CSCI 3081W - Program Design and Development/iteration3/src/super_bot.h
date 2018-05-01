/**
 * @file super_bot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SUPER_BOT_H_
#define SRC_SUPER_BOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/robot.h"
#include "src/motion_handler_super_bot.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/**
 * @brief A robot that moves around the arena and avoids collisions. Can be
 * frozen by the player and unfrozen by another robot.
 */
class SuperBot : public Robot {
 public:
  /**
   * @brief Initializes the SuperBot with the same ID as the robot it was
   * constructed from.
   */
  explicit SuperBot(const struct robot_params* const params, int in_id);

  entity_types get_entity_type(void) const { return kSuperBot; }

  std::string get_name(void) const {
    return "SuperBot" + std::to_string(id_);
  }
};

NAMESPACE_END(csci3081);

#endif  // SRC_SUPER_BOT_H_
