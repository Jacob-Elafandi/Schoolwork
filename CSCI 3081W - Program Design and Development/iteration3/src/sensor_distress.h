/**
 * @file sensor_distress.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_DISTRESS_H_
#define SRC_SENSOR_DISTRESS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/sensor.h"
#include "src/event_distress_call.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/

/**
 * @brief A representation of a distress sensor. Senses if any entity in a
 * defined range is outputting a distress signal, but does not sense direction.
 */
class SensorDistress : public Sensor {
 public:
  /**
   * @brief Sets the sensor to be activated by default.
   *
   * @param in_range The range of the sensor.
   */
  explicit SensorDistress(int in_range);

  /**
   * @brief Detects whether there is an entity in distress nearby.
   * If so, output is 1; if not, output is 0.
   */
  void Accept(EventDistressCall * e);

  /**
   * @brief Returns the maximum distance over which a distress call can be
   * picked up.
   */
  double get_range_of_view(void) const { return range_of_view_; }

  /**
   * @brief Return an enum denoting this sensor as a SensorDistress.
   */
  sensor_types get_sensor_type(void) const { return kSensorDistress; }

 private:
  double range_of_view_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_DISTRESS_H_
