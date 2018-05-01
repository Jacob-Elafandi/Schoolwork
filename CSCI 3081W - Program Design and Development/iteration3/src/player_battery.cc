/**
 * @file player_battery.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cmath>
#include "src/player_battery.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
double PlayerBattery::MotionDeplete(Position old_pos, Position new_pos) {
  // depletion = scalar * distance traveled
  double depletion = kLINEAR_SCALE_FACTOR * std::sqrt(
    std::pow(new_pos.x - old_pos.x, 2) + std::pow(new_pos.y - old_pos.y, 2));
  if (depletion > charge_) {
    charge_ = 0;
  } else {
    charge_ -= depletion;
  }
  return charge_;
} /* deplete() */

void PlayerBattery::BumpDeplete() {
  charge_ -= kBUMP_DEPLETION;
}

NAMESPACE_END(csci3081);
