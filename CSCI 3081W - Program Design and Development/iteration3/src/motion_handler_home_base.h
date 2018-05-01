/**
 * @file motion_handler_home_base.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_HOME_BASE_H_
#define SRC_MOTION_HANDLER_HOME_BASE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
 * @brief The handler for the home base's actuators. It makes the home base
 * change direction randomly at random intervals.
 */
class MotionHandlerHomeBase : public MotionHandler {
 public:
  /**
   * @brief Initializes the local variables to arbitrary values, to be
   * overwritten immediately after.
   */
  MotionHandlerHomeBase();
  // TODO(elafa009): make a constructor that initializes the variables directly

  /**
   * @brief Change the speed and direction according to the sensor readings.
   * May also change direction randomly.
   *
   * @param st touch sensor that can be activated and contains the point and
   * angle of contact
   */
  void UpdateVelocity(SensorTouch st);

  /**
   * @brief Set the percentage chance that the Home Base will change direction
   * at any given timestep.
   */
  void set_heading_change_odds(double hco) {
    heading_change_odds_ = hco;
  }

 private:
  /**
   * @brief The percentage chance that the home base will change direction
   * in any given timestep.
   */
  double heading_change_odds_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_HOME_BASE_H_
