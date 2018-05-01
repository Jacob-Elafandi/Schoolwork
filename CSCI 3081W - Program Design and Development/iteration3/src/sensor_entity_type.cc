/**
 * @file sensor_entity_type.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <limits>
#include "src/sensor_entity_type.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
SensorEntityType::SensorEntityType(double in_range) :
  Sensor(),
  range_of_view_(in_range) {
  set_output(kNone);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

void SensorEntityType::Accept(EventTypeEmit * e) {
    // Determine if the sensor should be activated or deactivated.
    if (e->get_entity_in_range()) {
      set_output(e->get_entity_type());
      e->EmitMessage();
    } else {
      set_output(kNone);
    }
}

NAMESPACE_END(csci3081);
