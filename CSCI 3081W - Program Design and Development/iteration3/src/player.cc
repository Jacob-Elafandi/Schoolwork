/**
 * @file player.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/player.h"
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Player::Player(const struct player_params* const params) :
  ArenaMobileEntity(params),
  motion_handler_(new MotionHandlerPlayer),
  params_(params),
  battery_(params->battery_max_charge) {
  motion_handler_->set_angle_delta(params->angle_delta);
  motion_handler_->set_speed_delta(params->speed_delta);
  motion_handler_->set_max_speed(params->max_speed);
  motion_handler_->set_speed(params->speed);
  motion_handler_->set_heading_angle(params->heading_angle);
  motion_handler_->set_max_frozen_timer(params->max_frozen_timer);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Player::TimestepUpdate() {
  Position old_pos = get_pos();
  // Update heading and speed as indicated by touch sensor
  double oldHeadingAngle = get_heading_angle();
  motion_handler_->UpdateVelocity(sensor_touch_);
  // Use velocity and position to update position (unless frozen)
  if (!motion_handler_->get_is_frozen()) {
    motion_behavior_.UpdatePosition(this);
  }
  if (sensor_touch_.get_entity_type() == kRechargeStation
    && sensor_touch_.get_output() == 1) {
    battery_.Recharge();  // If bumped into the recharge station
  } else {
    // If the player bumped into something, deplete battery
    if (get_heading_angle() != oldHeadingAngle) {
      battery_.BumpDeplete();
    }
    // Deplete battery as appropriate given distance and speed of movement
    battery_.MotionDeplete(old_pos, get_pos());
    std::cout << get_name() << " Battery level: " <<
      battery_.get_level() << std::endl;
  }
} /* TimestepUpdate() */

// User input commands to change heading or speed
void Player::Accept(EventCommand * e) {
  motion_handler_->AcceptCommand(e);
}

void Player::Reset() {
  ArenaMobileEntity::Reset();
  battery_.Reset();
  motion_handler_->Reset();
  motion_handler_->set_angle_delta(params_->angle_delta);
  motion_handler_->set_speed_delta(params_->speed_delta);
  motion_handler_->set_max_speed(params_->max_speed);
  motion_handler_->set_speed(params_->speed);
  motion_handler_->set_heading_angle(params_->heading_angle);
  motion_handler_->set_max_frozen_timer(params_->max_frozen_timer);
} /* Reset() */

void Player::ResetBattery(void) {
  battery_.Reset();
}

NAMESPACE_END(csci3081);
