/**
 * @file motion_handler_player.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_player.h"
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
MotionHandlerPlayer::MotionHandlerPlayer() :
  MotionHandler(),
  max_speed_(5),
  angle_delta_(10),
  speed_delta_(1),
  max_frozen_timer_(10),
  frozen_timer_(0) {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void MotionHandlerPlayer::Reset(void) {
  max_speed_ = 5;
  angle_delta_ = 10;
  speed_delta_ = 1;
  max_frozen_timer_ = 10;
  frozen_timer_ = 0;
}

void MotionHandlerPlayer::UpdateVelocity(SensorTouch st) {
  if (st.get_output() == 1 && st.get_entity_type() == kSuperBot) {
    frozen_timer_ = max_frozen_timer_;  // Freeze for a few frames
  } else if (frozen_timer_ > 0) {
    frozen_timer_ -= 1;
  } else {
    double oldHeadingAngle = get_heading_angle();
    MotionHandler::UpdateVelocity(st);
    if (oldHeadingAngle != get_heading_angle()) {  // Player bumped something
      set_speed(get_speed() - speed_delta_);
    }
  }
}

void MotionHandlerPlayer::AcceptCommand(EventCommand * e) {
  if (frozen_timer_ == 0) {
    switch (e->get_cmd()) {
      case COM_TURN_LEFT:
      set_heading_angle(get_heading_angle() - angle_delta_);  // Anticlockwise
      break;
      case COM_TURN_RIGHT:
      set_heading_angle(get_heading_angle() + angle_delta_);  // Clockwise
      break;
      case COM_SPEED_UP:
      set_speed(get_speed() + speed_delta_);
      break;
      case COM_SLOW_DOWN:
      set_speed(get_speed() - speed_delta_);
      break;
      default:
      std::cerr << "FATAL: bad actuator command" << std::endl;
      assert(0);
    } /* switch() */
  }
} /* accept_command() */

void MotionHandlerPlayer::AcceptCommand(enum event_commands event) {
  EventCommand e(event);
  AcceptCommand(&e);
}

void MotionHandlerPlayer::set_speed(double sp) {
  if (sp > max_speed_) {
    MotionHandler::set_speed(max_speed_);
  } else {
    MotionHandler::set_speed(sp);
  }
}

NAMESPACE_END(csci3081);
