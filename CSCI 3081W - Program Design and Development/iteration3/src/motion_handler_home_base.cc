/**
 * @file home_base.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_home_base.h"
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
MotionHandlerHomeBase::MotionHandlerHomeBase() : MotionHandler(),
  heading_change_odds_(2) {}

void MotionHandlerHomeBase::UpdateVelocity(SensorTouch st) {
  // Don't want to change angle randomly if the angle's already been changed
  // because of a collision. Compare the heading angles pre- and post-update.
  // If they're the same, roll to see if angle changes now.
  double old_heading_angle = get_heading_angle();
  MotionHandler::UpdateVelocity(st);
  if (get_heading_angle() == old_heading_angle &&
    rand() % 100 < heading_change_odds_) {
      set_heading_angle(rand() % 360);  // TODO(elafa009): improve thread safety
  }
}

NAMESPACE_END(csci3081);
