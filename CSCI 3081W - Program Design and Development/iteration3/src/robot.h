/**
 * @file robot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/arena_mobile_entity.h"
#include "src/sensor_proximity.h"
#include "src/sensor_touch.h"
#include "src/sensor_distress.h"
#include "src/sensor_entity_type.h"
#include "src/motion_handler_robot.h"
#include "src/robot_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/**
 * @brief A robot that moves around the arena and avoids collisions. Can be
 * frozen by the player and unfrozen by another robot.
 */
class Robot : public ArenaMobileEntity {
 public:
  /**
   * @brief Initializes the robot and gives it a unique ID.
   */
  explicit Robot(const struct robot_params* const params);

  void Reset();
  virtual void TimestepUpdate();
  entity_types get_entity_type(void) const { return kRobot; }

  /**
   * @brief Returns the unique ID of the robot.
   */
  uint get_id(void) const { return id_; }

  /**
   * @brief Return true if the robot is frozen (because of contact with Player),
   * and false if not.
   */
  bool get_is_frozen() const { return motion_handler_->get_is_frozen(); }

  double get_heading_angle(void) const {
    return motion_handler_->get_heading_angle(); }
  void set_heading_angle(double ha) { motion_handler_->set_heading_angle(ha); }
  double get_speed(void) const { return motion_handler_->get_speed(); }
  void set_speed(double sp) { motion_handler_->set_speed(sp); }

  /**
   * @brief If true, the Robot has bumped the Home Base, and should be turned
   * into a SuperBot.
   */
  bool get_touched_home_base(void) const {
    return motion_handler_->get_touched_home_base(); }

  /**
   * @brief Gets the parameter structure that was used to construct the Robot.
   * This function is called when this Robot is being replaced with a SuperBot,
   * to provide parameters to the new SuperBot.
   */
  const robot_params* get_params(void) const { return params_; }

  std::string get_name(void) const {
    return "Robot" + std::to_string(id_);
  }

 protected:
  /**
   * @brief The unique ID of the robot.
   */
  uint id_;

  /**
   * @brief The ID assigned to the next Robot to be constructed.
   */
  static uint next_id_;

  /**
   * @brief The handler for the Robot's motion. Gets overwritten by a
   * MotionHandlerSuperBot in the derived class SuperBot.
   */
  MotionHandlerRobot* motion_handler_;

  /**
   * @brief A proximity sensor on the left side of the robot. Causes the robot
   * to slow down and turn when something is detected in its range and field.
   */
  SensorProximity sensor_proximity_left_;

  /**
   * @brief A proximity sensor on the right side of the robot. Causes the robot
   * to slow down and turn when something is detected in its range and field.
   */
  SensorProximity sensor_proximity_right_;

  /**
   * @brief A distress sensor. If a frozen Robot is nearby, this sensor will
   * activate, and tell the motion handler to ignore the proximity sensors
   * until there is no longer a distress call in range.
   */
  SensorDistress sensor_distress_;

  /**
   * @brief A sensor that determines the type of the nearest entity, so that
   * the Robot knows what objects to NOT avoid with its proximity sensors.
   * Normal Robots don't avoid HomeBase, so they can turn into SueprBots.
   * SuperBots don't avoid Player, so they can freeze Player.
   */
  SensorEntityType sensor_entity_type_;

 private:
  const robot_params* params_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_H_
