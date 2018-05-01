/**
 * @file motion_handler_player.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_PLAYER_H_
#define SRC_MOTION_HANDLER_PLAYER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/event_command.h"
#include "src/motion_handler.h"
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
 * @brief The handler for the player's actuators, which in this case are the two
 * wheel actuators. Its main job is to translate the directional commands from
 * the user into the appropriate differential drive wheel speeds.
 *
 * MotionHandlerPlayer manages the modification to the velocity based on
 * user input and collisions.
 * MotionBehavior translates velocity and position to a new position.
 * Both of these are straightforward, but the framework allows for more
 * sophisticated models of motion in which each wheel has distinct speed.
 *
 * For this iteration, both wheels are always going at maximum speed, and
 * cannot be controlled independently.
 */
class MotionHandlerPlayer : public MotionHandler {
 public:
  /**
   * @brief Initializes the local variables to arbitrary values, to be
   * overwritten immediately after.
   */
  MotionHandlerPlayer();
  // TODO(elafa009): make a constructor that initializes the variables directly

  /**
   * @brief Reset the actuators to their newly constructed/un-commanded state.
   */
  void Reset(void);

  /**
  * @brief Change the speed and direction according to the sensor readings.
  *
  * @param st touch sensor that can be activated and contains point-of-contact
  */
  void UpdateVelocity(SensorTouch st);

  /**todo
   * @brief Command from user keypress via the viewer.
   *
   * @param e An event containing the command.
   */
  void AcceptCommand(EventCommand * e);

  /**
   * @brief Command from user keypress via the viewer. (Deprecated)
   *
   * @param event The command.
   */
  void AcceptCommand(enum event_commands event);

  /**
   * @brief Return the number of degrees that the player will turn by
   * when commanded to do so by user keypress.
   */
  double get_angle_delta() const { return angle_delta_; }

  /**
   * @brief Assign a new number of degrees that the player will turn by
   * when commanded to do so by user keypress.
   *
   * @param ad The new angle delta.
   */
  void set_angle_delta(double ad) { angle_delta_ = ad; }

  /**
   * @brief Return the change in speed, in pixels per timestep, that the
   * player undertakes when sped up or slowed down by user keypress.
   */
  double get_speed_delta() const { return speed_delta_; }

  /**
   * @brief Assigns a new change in speed that the player will undertake
   * when commanded to do so by user keypress.
   *
   * @param sd The new speed delta.
   */
  void set_speed_delta(double sd) { speed_delta_ = sd; }

  /**
   * @brief Returns the number of frames that the Player will be frozen for
   * upon contact with a SuperBot.
   */
  double get_max_frozen_timer() const { return max_frozen_timer_; }

  /**
   * @brief Assigns the number of frames that the Player will be frozen for
   * upon contact with a SuperBot.
   *
   * @param mft The number of frames.
   */
  void set_max_frozen_timer(double mft) { max_frozen_timer_ = mft; }

  /**
   * @brief Returns true if the robot is frozen because of colliding with
   * a SuperBot.
   */
  bool get_is_frozen(void) const { return frozen_timer_ > 0; }

  /**
   * @brief Sets the speed equal to the input value, unless it's negative
   * (in which case the speed becomes zero) or it's greater than the max
   * speed (in which case the speed becomes the max speed).
   */
  void set_speed(double sp);

  /**
   * @brief Gets the maximum speed that the player can be accelerated to by
   * keypresses.
   */
  double get_max_speed() const { return max_speed_; }

  /**
   * @brief Sets the maximum speed that the player can be accelerated to
   * by keypresss.
   */
  void set_max_speed(double ms) { max_speed_ = ms; }


 private:
  double max_speed_;
  double angle_delta_;
  double speed_delta_;
  uint max_frozen_timer_;
  uint frozen_timer_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_PLAYER_H_
