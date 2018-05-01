/**
 * @file motion_handler.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_H_
#define SRC_MOTION_HANDLER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"
#include "src/event_commands.h"
#include "src/player_params.h"
#include "src/sensor_touch.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
 * @brief The handler for the actuators of an ArenaMobileEntity, which in
 * this case are the two wheel actuators.
 *
 * MotionHandler manages the modification to the velocity based on
 * collisions.
 * MotionBehavior translates velocity and position to a new position.
 * Both of these are straightforward, but the framework allows for more
 * sophisticated models of motion in which each wheel has distinct speed.
 *
 * For this iteration, both wheels are always going at maximum speed, and
 * cannot be controlled independently.
 *
 * MotionHandlerPlayer is a subclass of MotionHandler that makes modifications
 * based on user input. It is used in Player.
 */
class MotionHandler {
 public:
  /**
   * @brief Initializes the local variables to arbitrary values, to be
   * overwritten immediately after.
   */
  MotionHandler();
  // TODO(elafa009): make a constructor that initializes the variables directly
  virtual ~MotionHandler(void) {}

  /**
   * @brief Reset the actuators to their newly constructed/un-commanded state.
   */
  virtual void Reset(void);

  /**
   * @brief Change the speed and direction according to the sensor readings.
   *
   * @param st touch sensor that can be activated and contains point-of-contact
   * and angle-of-contact
   */
  virtual void UpdateVelocity(SensorTouch st);

  /**
   * @brief Return the current speed of the entity, in pixels per timestep.
   */
  double get_speed() const { return speed_; }
  /**
   * @brief Assigns a new speed to the entity.
   *
   * @param sp The new speed of the entity, in pixels per timestep.
   */
  virtual void set_speed(double sp);

  /**
   * @brief Returns the angle that the entity is currently travelling along,
   * clockwise from "east" in degrees (0-359).
   */
  double get_heading_angle() const { return heading_angle_;}
  /**
   * @brief Assigns a new heading angle to the entity.
   *
   * @param ha The new heading angle for the entity, clockwise from "east"
   * in degrees (0-359).
   */
  void set_heading_angle(double ha) {
    heading_angle_ = fmod(ha + 360.0, 360.0); }

 private:
  double heading_angle_;
  double speed_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_H_
