/**
 * @file motion_handler.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler.h"
#include <cmath>
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
MotionHandler::MotionHandler() :
  heading_angle_(5),
  speed_(3) {
}

void MotionHandler::Reset(void) {
  heading_angle_ = 45;
  speed_ = 3;
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void MotionHandler::UpdateVelocity(SensorTouch st) {
  std::cout << "Heading angle: " << get_heading_angle() << std::endl;
  // If the difference in the angle of contact and the heading angle is
  // greater than 90 degrees, the collision is at the rear. To prevent getting
  // stuck in other objects, collisions at the rear have no effect.
  if (st.get_output() == 1 &&
      (std::abs(st.get_angle_of_contact() - heading_angle_) <= 90 ||
       std::abs(st.get_angle_of_contact() - heading_angle_ + 360) <= 90 ||
       std::abs(st.get_angle_of_contact() - heading_angle_ - 360) <= 90)) {
    // Bounce off the object/wall that this entity has collided with
    set_heading_angle(2 * st.get_angle_of_contact() - heading_angle_ + 180);
    std::cout << "Collision detected" << std::endl;
  } else if (st.get_output() == 1) {
    // Debugging purposes
    std::cout << "False collision. Angle of contact = " <<
      st.get_angle_of_contact() << ", Heading angle = " <<
      heading_angle_ << std::endl;
  }
}

void MotionHandler::set_speed(double sp) {
  if (sp < 0) {
    speed_ = 0;
  } else {
    speed_ = sp;
  }
}

NAMESPACE_END(csci3081);
