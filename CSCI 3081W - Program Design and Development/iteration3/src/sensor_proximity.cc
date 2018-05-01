/**
 * @file sensor_proximity.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <limits>
#include "src/sensor_proximity.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
SensorProximity::SensorProximity(double in_range, double in_field,
    bool in_is_left):
  Sensor(),
  range_of_view_(in_range),
  field_of_view_(in_field),
  is_left_(in_is_left) {
  set_output(-1);
}

void SensorProximity::Reset() {
  Sensor::Reset();
  set_output(-1);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

void SensorProximity::Accept(EventProximity * e) {
    // Determine if the sensor should be activated or deactivated.
    if (e->get_entity_in_view()) {
      set_output(e->get_distance_to_entity());
      e->EmitMessage();
    } else {
      set_output(-1);
    }
}

NAMESPACE_END(csci3081);
