/**
 * @file arena_mobile_entity_params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_MOBILE_ENTITY_PARAMS_H_
#define SRC_ARENA_MOBILE_ENTITY_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena_entity_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/

/**
 * @brief The parameters used in construction of all mobile entities.
 */
struct arena_mobile_entity_params : public arena_entity_params {
  arena_mobile_entity_params(void) :
    arena_entity_params(),
    collision_delta(),
    heading_angle(),
    speed() {}

  /**
   * @brief Copy an existing arena_mobile_entity_params struct.
   */
  explicit arena_mobile_entity_params(const arena_mobile_entity_params* p) :
    arena_entity_params(p),
    collision_delta(p->collision_delta),
    heading_angle(p->heading_angle),
    speed(p->speed) {}

  /**
   * @brief The entity must be this distance or closer to another entity or
   * the wall for a collision to be detected
   */
  double collision_delta;

  /**
   * @brief The direction the entity is initially travelling, clockwise
   * from east in degrees (0-359)
   */
  double heading_angle;

  /**
   * @brief The initial speed of the entity, in pixels per timestep.
   */
  double speed;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_MOBILE_ENTITY_PARAMS_H_
