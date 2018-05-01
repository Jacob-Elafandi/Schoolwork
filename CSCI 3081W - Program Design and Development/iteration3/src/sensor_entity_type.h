/**
 * @file sensor_entity_type.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_ENTITY_TYPE_H_
#define SRC_SENSOR_ENTITY_TYPE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/sensor.h"
#include "src/event_type_emit.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/

/**
 * @brief Type of entity emitting signal can be sensed when it is within a
 * defined range, but the direction of the signal cannot be determined.
 */
class SensorEntityType : public Sensor {
 public:
  /**
   * @brief Sets the sensor to be deactivated by default.
   *
   * @param in_range The range of the sensor.
   */
  explicit SensorEntityType(double in_range);

  /**
   * @brief Detects the type of entity. Sets output to an enum.
   */
  void Accept(EventTypeEmit * e);

  /**
   * @brief Returns the range of view of the sensor.
   */
  double get_range_of_view(void) const { return range_of_view_; }

  /**
   * @brief Return an enum denoting this sensor as a SensorEntityType.
   */
  sensor_types get_sensor_type(void) const { return kSensorEntityType; }

 private:
  double range_of_view_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_ENTITY_TYPE_H_
