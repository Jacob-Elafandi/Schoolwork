/**
 * @file sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"
#include "src/position.h"
#include "src/event_base_class.h"
#include "src/sensor_types.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
 * @brief A representation of a general sensor. Subclasses can be made
 * "active" by events (e.g. bumping into something) and will then send
 * signals to arena entities.
 */
class Sensor {
 public:
  /**
   * @brief Sets the sensor to be deactivated by default.
   */
  Sensor(void): output_(0) {}

  virtual ~Sensor(void) {}

  /**
   * @brief Return the Sensor's output.
   */
  int get_output(void) const { return output_; }

  /**
   * @brief Set what the Sensor will output.
   *
   * @param value The value to be later output.
   */
  void set_output(int value) { output_ = value; }

  /**
   * @brief Reset the sensor to its newly constructed state.
   */
  virtual void Reset(void) { output_ = 0; }

  /**
   * @brief Get the type of the sensor (touch, proximity, ...)
   */
  virtual sensor_types get_sensor_type(void) const = 0;

 private:
  int output_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_H_
