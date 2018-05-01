/**
 * @file event_collision.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/event_collision.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructor
 ******************************************************************************/
EventCollision::EventCollision() :
  collided_(false),
  point_of_contact_(0, 0),
  angle_of_contact_(0),
  type_of_entity_(kNone) {
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void EventCollision::EmitMessage(void) {
  printf("Collision event with object type %d at point %f %f. Angle %f\n",
  type_of_entity_, point_of_contact_.x, point_of_contact_.y, angle_of_contact_);
} /* EmitMessage() */

NAMESPACE_END(csci3081);
