/**
 * @file event_distress_call.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/event_distress_call.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructor
 ******************************************************************************/
EventDistressCall::EventDistressCall() :
  calling_(false),
  position_of_distress_call_(0, 0) {
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void EventDistressCall::EmitMessage(void) {
  printf("Distress call at point %f %f\n",
  position_of_distress_call_.x, position_of_distress_call_.y);
} /* EmitMessage() */

NAMESPACE_END(csci3081);
