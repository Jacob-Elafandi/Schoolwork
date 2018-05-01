/**
 * @file motion_handler_robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_robot.h"
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
MotionHandlerRobot::MotionHandlerRobot() :
  MotionHandler(),
  max_speed_(get_speed()),
  is_frozen_(false),
  touched_home_base_(false) {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

void MotionHandlerRobot::UpdateVelocity(SensorProximity sensor_proximity_left,
                                        SensorProximity sensor_proximity_right,
                                        SensorDistress sensor_distress,
                                        SensorEntityType sensor_entity_type,
                                        SensorTouch sensor_touch) {
  if (sensor_touch.get_output() == 1) {
    if (sensor_touch.get_entity_type() == kPlayer) {
      is_frozen_ = true;
      set_speed(0);
    } else if (sensor_touch.get_entity_type() == kRobot ||
        sensor_touch.get_entity_type() == kSuperBot) {
      is_frozen_ = false;
    } else if (sensor_touch.get_entity_type() == kHomeBase) {
      is_frozen_ = false;
      touched_home_base_ = true;
    }
  }

  if (!is_frozen_) {
    set_speed(max_speed_);  // Default if nothing sensed
    MotionHandler::UpdateVelocity(sensor_touch);  // Bounce off entities/walls
    if (sensor_distress.get_output() == 0 &&
        sensor_entity_type.get_output() != kHomeBase) {
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
}

void MotionHandlerRobot::Reset() {
  MotionHandler::Reset();
  set_speed(max_speed_);
  is_frozen_ = false;
  touched_home_base_ = false;
}

void MotionHandlerRobot::set_speed(double sp) {
  if (sp > max_speed_) {
    MotionHandler::set_speed(max_speed_);
  } else {
    MotionHandler::set_speed(sp);
  }
}

NAMESPACE_END(csci3081);
