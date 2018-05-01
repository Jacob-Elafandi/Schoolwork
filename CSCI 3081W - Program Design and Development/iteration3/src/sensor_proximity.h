/**
 * @file sensor_proximity.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_PROXIMITY_H_
#define SRC_SENSOR_PROXIMITY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/sensor.h"
#include "src/event_proximity.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/

/**
 * @brief A representation of a proximity sensor. Each Robot has two,
 * represented as two cones in front of the Robot, bordering at the middle.
 * Output is the distance to an obstacle, or -1 if none is sensed.
 */
class SensorProximity : public Sensor {
 public:
  /**
   * @brief Constructs the sensor as being deactivated by default and assigns
   * range and field of view.
   *
   * @param in_range The max distance in pixels that the sensor can sense
   * @param in_field The max angle from center that the sensor can sense
   * @param in_is_left True if the sensor is on the left side of the entity.
   */
  SensorProximity(double in_range, double in_field, bool in_is_left);

  void Reset();

  /**
   * @brief Compute a new reading based on the proximity event.
   */
  void Accept(EventProximity * e);

  /**
   * @brief Returns true if this is the proximity sensor on the left side of
   * the robot, and false if it's on the right.
   */
  bool get_is_left(void) const { return is_left_; }

  /**
   * @brief Returns the sensor's field of view.
   */
  double get_field_of_view(void) const { return field_of_view_; }

  /**
   * @brief Returns the sensor's range of view.
   */
  double get_range_of_view(void) const { return range_of_view_; }

  /**
   * @brief Return an enum denoting this sensor as a SensorProximity.
   */
  sensor_types get_sensor_type(void) const { return kSensorProximity; }

 private:
  double range_of_view_;
  double field_of_view_;
  bool is_left_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_PROXIMITY_H_
