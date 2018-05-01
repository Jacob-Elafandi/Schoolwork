/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot.h"
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Static Variables
 ******************************************************************************/
uint Robot::next_id_ = 0;

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Robot::Robot(const struct robot_params* const params) :
  ArenaMobileEntity(params),
  id_(-1),
  motion_handler_(new MotionHandlerRobot),
  sensor_proximity_left_(params->proximity_range,
                         params->proximity_field, true),
  sensor_proximity_right_(params->proximity_range,
                          params->proximity_field, false),
  sensor_distress_(params->distress_range),
  sensor_entity_type_(params->entity_type_range),
  params_(params) {
  id_ = next_id_++;
  motion_handler_->set_speed(params->speed);
  motion_handler_->set_heading_angle(params->heading_angle);
  motion_handler_->set_max_speed(params->speed);
  sensors_.push_back(&sensor_proximity_left_);
  sensors_.push_back(&sensor_proximity_right_);
  sensors_.push_back(&sensor_distress_);
  sensors_.push_back(&sensor_entity_type_);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Robot::TimestepUpdate() {
  // Update heading and speed as indicated by sensors
  motion_handler_->UpdateVelocity(sensor_proximity_left_,
                                  sensor_proximity_right_,
                                  sensor_distress_,
                                  sensor_entity_type_,
                                  sensor_touch_);
  // Use velocity and position to update position
  motion_behavior_.UpdatePosition(this);
} /* TimestepUpdate() */

void Robot::Reset() {
  ArenaMobileEntity::Reset();
  motion_handler_->Reset();
  motion_handler_->set_speed(params_->speed);
  motion_handler_->set_heading_angle(params_->heading_angle);
  motion_handler_->set_max_speed(params_->speed);
  sensor_proximity_left_.Reset();
  sensor_proximity_right_.Reset();
  sensor_distress_.Reset();
  sensor_entity_type_.Reset();
} /* Reset() */

NAMESPACE_END(csci3081);
