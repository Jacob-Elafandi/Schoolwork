/**
 * @file home_base.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/home_base.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
HomeBase::HomeBase(const struct home_base_params* const params) :
  ArenaMobileEntity(params),
  params_(params),
  motion_handler_(new MotionHandlerHomeBase) {
  motion_handler_->set_speed(params->speed);
  motion_handler_->set_heading_angle(params->heading_angle);
  motion_handler_->set_heading_change_odds(params_->heading_change_odds);
}

void HomeBase::TimestepUpdate() {
  motion_handler_->UpdateVelocity(sensor_touch_);
  motion_behavior_.UpdatePosition(this);
}

void HomeBase::Reset() {
  ArenaMobileEntity::Reset();
  motion_handler_->set_speed(params_->speed);
  motion_handler_->set_heading_angle(params_->heading_angle);
  motion_handler_->set_heading_change_odds(params_->heading_change_odds);
}

NAMESPACE_END(csci3081);
