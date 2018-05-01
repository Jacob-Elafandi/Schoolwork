/**
 * @file motion_handler_robot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_ROBOT_H_
#define SRC_MOTION_HANDLER_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler.h"
#include "src/sensor_proximity.h"
#include "src/sensor_touch.h"
#include "src/sensor_distress.h"
#include "src/sensor_entity_type.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
 * @brief The handler for the robot's actuators, which in this case are the two
 * wheel actuators.
 *
 * Avoids non-HomeBase entities and walls unless distress call sensed. Bounces
 * on contact with entities and walls.
 */
class MotionHandlerRobot : public MotionHandler {
 public:
  /**
   * @brief Initializes the local variables to arbitrary values, to be
   * overwritten immediately after.
   */
  MotionHandlerRobot();
  // TODO(elafa009): make a constructor that initializes the variables directly

  void Reset();

  /**
   * @brief Change the speed and direction according to the sensor readings.
   */
  virtual void UpdateVelocity(SensorProximity sensor_proximity_left,
                      SensorProximity sensor_proximity_right,
                      SensorDistress sensor_distress,
                      SensorEntityType sensor_entity_type,
                      SensorTouch sensor_touch);

  /**
   * @brief Returns true if the Robot is frozen.
   */
  bool get_is_frozen(void) const { return is_frozen_; }

  /**
   * @brief Sets the speed that the Robot will move at when not frozen and when
   * not slowing down because of proximity detection. Should initially be set
   * equal to the speed.
   */
  void set_max_speed(double ms) { max_speed_ = ms; }

  void set_speed(double sp);

  /**
   * @brief If true, the Robot has bumped the Home Base, and should be turned
   * into a SuperBot.
   */
  bool get_touched_home_base(void) const { return touched_home_base_; }

 protected:
  /**
   * @brief The speed the robot will travel at when not frozen by Player and
   * when not slowing down due to proximity detection.
   */
  double max_speed_;

 private:
  bool is_frozen_;
  bool touched_home_base_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_ROBOT_H_
