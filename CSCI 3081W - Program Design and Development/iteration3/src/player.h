/**
 * @file player.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/player_battery.h"
#include "src/arena_mobile_entity.h"
#include "src/event_collision.h"
#include "src/event_command.h"
#include "src/motion_handler_player.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/**
 * @brief A player that moves around the arena and responds to user commands.
 */
class Player : public ArenaMobileEntity {
 public:
  /**
   * @brief Initializes the player and gives it a unique ID.
   */
  explicit Player(const struct player_params* const params);
  /**
   * @brief Resets the battery to its initial state.
   */
  void ResetBattery(void);
  void Reset();
  void TimestepUpdate();
  entity_types get_entity_type(void) const { return kPlayer; }

  /**
   * @brief Causes the player to accept a user command
   * (e.g. turn left, speed up)
   *
   * @param e The event containing the command to be accepted.
   */
  void Accept(EventCommand * e);

  double get_heading_angle(void) const {
    return motion_handler_->get_heading_angle(); }
  void set_heading_angle(double ha) { motion_handler_->set_heading_angle(ha); }
  double get_speed(void) const { return motion_handler_->get_speed(); }
  void set_speed(double sp) { motion_handler_->set_speed(sp); }

  /**
   * @brief Returns the current battery level of the player.
   */
  double get_battery_level(void) const { return battery_.get_level(); }
  /**
   * @brief Returns the maximum battery level of the player.
   */
  double get_max_battery_level(void) const { return battery_.get_max_level(); }

  std::string get_name(void) const {
    return "Player";
  }

 private:
  MotionHandlerPlayer* motion_handler_;
  const player_params* params_;
  PlayerBattery battery_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_PLAYER_H_
