/**
 * @file motion_behavior.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_BEHAVIOR_H_
#define SRC_MOTION_BEHAVIOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <Eigen/Dense>
#include "src/common.h"
#include "src/position.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/**
 * @brief A component class for an ArenaMobileEntity that translates its
 * velocity and position to a new position.
 */
class MotionBehavior {
 public:
  MotionBehavior(void) {}

  /**
   * @brief Update the position for a Player, based on its current
   * position and velocity.
   *
   * MotionHandlerPlayer manages the modification to the velocity based on
   * user input and collisions.
   * MotionBehavior translates velocity and position to a new position.
   * Both of these are straightforward, but the framework allows for more
   * sophisticated models of motion in which each wheel has distinct speed.
   *
   * @param ent The entitity to update.
   */
  void UpdatePosition(class ArenaMobileEntity * const ent);
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_BEHAVIOR_H_
