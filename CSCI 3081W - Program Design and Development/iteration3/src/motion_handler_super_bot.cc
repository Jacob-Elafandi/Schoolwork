/**
 * @file motion_handler_super_bot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_super_bot.h"
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
MotionHandlerSuperBot::MotionHandlerSuperBot() :
  MotionHandlerRobot() {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

void MotionHandlerSuperBot::UpdateVelocity(
                                        SensorProximity sensor_proximity_left,
                                        SensorProximity sensor_proximity_right,
                                        SensorDistress sensor_distress,
                                        SensorEntityType sensor_entity_type,
                                        SensorTouch sensor_touch) {
  set_speed(max_speed_);  // Default if nothing sensed
  MotionHandler::UpdateVelocity(sensor_touch);  // Bounce off entities/walls
  if (sensor_distress.get_output() == 0 &&
      sensor_entity_type.get_output() != kPlayer) {
    if (sensor_proximity_left.get_output() != -1) {
      // Slow and turn right
      set_speed(max_speed_ * sensor_proximity_left.get_output()
                / sensor_proximity_left.get_range_of_view());
      set_heading_angle(get_heading_angle() + 5);
    } else if (sensor_proximity_right.get_output() != -1) {
      // Slow and turn left
      set_speed(max_speed_ * sensor_proximity_right.get_output()
                / sensor_proximity_right.get_range_of_view());
      set_heading_angle(get_heading_angle() - 5);
    }
  }
}

NAMESPACE_END(csci3081);
