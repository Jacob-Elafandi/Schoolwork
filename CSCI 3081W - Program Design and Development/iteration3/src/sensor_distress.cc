/**
 * @file sensor_distress.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <limits>
#include "src/sensor_distress.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
SensorDistress::SensorDistress(int in_range) :
  Sensor(),
  range_of_view_(in_range) {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

void SensorDistress::Accept(EventDistressCall * e) {
    // Determine if the sensor should be activated or deactivated.
    if (e->get_calling()) {
      set_output(1);
      e->EmitMessage();
    } else {
      set_output(0);
    }
}

NAMESPACE_END(csci3081);
