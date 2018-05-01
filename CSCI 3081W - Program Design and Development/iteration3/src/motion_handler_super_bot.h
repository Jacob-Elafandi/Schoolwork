/**
 * @file motion_handler_super_bot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_SUPER_BOT_H_
#define SRC_MOTION_HANDLER_SUPER_BOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_robot.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
 * @brief The handler for the SuperBot's actuators, which in this case are the
 * two wheel actuators.
 *
 * Avoids non-Player entities and walls unless distress call sensed. Bounces
 * on contact with entities and walls.
 */
class MotionHandlerSuperBot : public MotionHandlerRobot {
 public:
  /**
   * @brief Initializes the local variables to arbitrary values, to be
   * overwritten immediately after.
   */
  MotionHandlerSuperBot();
  // TODO(elafa009): make a constructor that initializes the variables directly

  /**
   * @brief Change the speed and direction according to the sensor readings.
   */
  void UpdateVelocity(SensorProximity sensor_proximity_left,
                      SensorProximity sensor_proximity_right,
                      SensorDistress sensor_distress,
                      SensorEntityType sensor_entity_type,
                      SensorTouch sensor_touch);
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_SUPER_BOT_H_
