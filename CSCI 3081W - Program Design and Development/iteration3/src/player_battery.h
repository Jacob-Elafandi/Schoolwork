/**
 * @file player_battery.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_PLAYER_BATTERY_H_
#define SRC_PLAYER_BATTERY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"
#include "src/position.h"
#include "src/event_collision.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
 * @brief The battery for a player.
 */
class PlayerBattery {
 public:
  /**
   * @brief Initializes the battery at full charge.
   *
   * @param max_charge The maximum level that the battery can be charged to.
   */
  explicit PlayerBattery(double max_charge) : charge_(max_charge),
                                             max_charge_(max_charge) {}

  /**
   * @brief The amount of energy consumed by the player is directly proportional
   * to its linear speed, with a scaling factor.
   */
  double kLINEAR_SCALE_FACTOR = 0.01;

  /**
   * @brief The amount of battery that is drained when the player bumps into
   * an obstacle or the wall.
   */
  double kBUMP_DEPLETION = 5;

  /**
   * @brief Get the current battery level.
   */
  double get_level(void) const { return charge_; }

  /**
   * @brief Get the maximum battery level.
   */
  double get_max_level(void) const { return max_charge_; }

  /**
   * @brief Handle a recharge event by instantly restoring the player's battery
   * to its maximum value.
   */
  void Recharge(void) { charge_ = max_charge_; }

  /**
   * @brief Reset the player's battery to its initial (full) state.
   */
  void Reset(void) { Recharge(); }

  /**
   * @brief Calculate the new battery level based on the current speed.
   *
   * @param old_pos The player's position before the timestep.
   * @param new_pos The player's position after the timestep.
   *
   * @return The updated battery level.
   */
  double MotionDeplete(Position old_pos, Position new_pos);

  /**
   * @brief Inform the battery a collision occurred, and deplete accordingly.
   */
  void BumpDeplete(void);

 private:
  double charge_;
  double max_charge_;
};

NAMESPACE_END(csci3081);

#endif   // SRC_PLAYER_BATTERY_H_
