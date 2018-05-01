/**
 * @file super_bot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/super_bot.h"
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
SuperBot::SuperBot(const struct robot_params* const params, int in_id) :
  Robot(params) {
  id_ = in_id;
  motion_handler_ = new MotionHandlerSuperBot;
  motion_handler_->set_speed(params->speed);
  motion_handler_->set_heading_angle(params->heading_angle);
  motion_handler_->set_max_speed(params->speed);
  next_id_--;  // Don't want to skip IDs if a new normal Robot gets constructed
}

NAMESPACE_END(csci3081);
