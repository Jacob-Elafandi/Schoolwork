/**
 * @file sensor_touch.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_TOUCH_H_
#define SRC_SENSOR_TOUCH_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <utility>

#include "src/event_collision.h"
#include "src/sensor.h"
#include "src/entity_types.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/

/**
 * @brief A representation of a touch sensor. Touch or "bump" sensors are
 * "active" when pressed. For our purposes, imagine a series of bump sensors
 * around the perimeter of the player. A collision will activate the sensor
 * at a particular point of contact, which translates to an angle of contact
 *
 */
class SensorTouch : public Sensor {
 public:
  /**
   * @brief Constructs the sensor as being deactivated by default
   * and assigns arbitrary values to the point and angle of contact.
   *
   * @param in_collision_delta The distance from the robot over which collisions
   * will be accepted.
   */
  explicit SensorTouch(double in_collision_delta);

  /**
   * @brief Returns the point that the most recent collision took place at.
   */
  Position get_point_of_contact() { return point_of_contact_; }

  /**
   * @brief Returns the angle of the most recent collision.
   *
   * Angle is with respect to the center of the entity this sensor belongs to,
   * clockwise from east in degrees (0-359).
   */
  double get_angle_of_contact(void) const { return angle_of_contact_; }

  /**
   * @brief Compute a new reading based on the collision event.
   *
   * Note that collision events can report "no collision" which
   * will inactivate the sensor.
   */
  void Accept(EventCollision * e);

  /**
   * @brief Returns the maximum distance, in pixels, that must be between the
   * entity and another entity (or the wall) for a collision to be detected.
   */
  double get_collision_delta(void) const { return collision_delta_; }

  /**
   * @brief Return the type of the entity that has been collided with.
   */
  entity_types get_entity_type(void) const { return type_of_entity_; }

  /**
   * @brief Return an enum denoting this sensor as a SensorTouch.
   */
  sensor_types get_sensor_type(void) const { return kSensorTouch; }

 private:
  Position point_of_contact_;
  double angle_of_contact_;
  double collision_delta_;
  entity_types type_of_entity_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_TOUCH_H_
