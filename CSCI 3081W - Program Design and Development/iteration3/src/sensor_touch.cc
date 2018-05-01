/**
 * @file sensor_touch.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <limits>
#include "src/sensor_touch.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
SensorTouch::SensorTouch(double in_collision_delta) :
  Sensor(),
  point_of_contact_(0, 0),
  angle_of_contact_(0),
  collision_delta_(in_collision_delta),
  type_of_entity_(kNone) {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

void SensorTouch::Accept(EventCollision * e) {
    // Determine if the sensor should be activated or deactivated.
    if (e->get_collided()) {
      set_output(1);
      point_of_contact_ = e->get_point_of_contact();
      angle_of_contact_ = e->get_angle_of_contact();
      type_of_entity_ = e->get_entity_type();
      e->EmitMessage();
    } else {
      set_output(0);
    }
}

NAMESPACE_END(csci3081);
