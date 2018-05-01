/**
 * @file event_proximity.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/event_proximity.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructor
 ******************************************************************************/
EventProximity::EventProximity() :
  entity_in_view_(false),
  distance_to_entity_(0),
  position_of_entity_(0, 0) {
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void EventProximity::EmitMessage(void) {
  printf("Entity detected at distance %f\n",
  distance_to_entity_);
} /* EmitMessage() */

NAMESPACE_END(csci3081);
