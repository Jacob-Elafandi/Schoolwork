/**
 * @file robot_params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_PARAMS_H_
#define SRC_ROBOT_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena_mobile_entity_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/

/**
 * @brief The parameters used in construction of a robot.
 *
 * At present, there are no parameters for a robot that are not
 * also parameters for any ArenaMobileEntity.
 */
struct robot_params : public arena_mobile_entity_params {
  robot_params(void) :
    arena_mobile_entity_params(),
    proximity_range(),
    proximity_field(),
    distress_range(),
    entity_type_range(),
    super_color() {}

  /**
   * @brief Copy an existing robot_params struct.
   */
  explicit robot_params(const robot_params* p) :
    arena_mobile_entity_params(p),
    proximity_range(p->proximity_range),
    proximity_field(p->proximity_field),
    distress_range(p->distress_range),
    entity_type_range(p->entity_type_range),
    super_color(p->super_color) {}

  /**
   * @brief The maximum distance, in pixels, that an entity can be detected
   * out from the robot by the proximity sensors.
   */
  double proximity_range;

  /**
   * @brief The angle off from center, in degrees, that each of the robot's
   * two proximity sensors covers.
   */
  double proximity_field;

  /**
   * @brief The radius from the center of the robot that the distress sensor
   * will sense a signal within.
   */
  double distress_range;

  /**
   * @brief The radius from the center of the robot that the entity type sensor
   * will sense a signal within.
   */
  double entity_type_range;

  /**
   * @brief The color that the Robot will turn when it's converted into
   * a SuperBot.
   */
  Color super_color;
};


NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_PARAMS_H_
