/**
 * @file sensor_types.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */

#ifndef SRC_SENSOR_TYPES_H_
#define SRC_SENSOR_TYPES_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/
/**
 * @brief The types of sensors an entity can have.
 */
enum sensor_types {
  kSensorTouch,
  kSensorProximity,
  kSensorDistress,
  kSensorEntityType
};
NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_TYPES_H_
