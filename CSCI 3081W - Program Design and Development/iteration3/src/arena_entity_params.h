/**
 * @file arena_entity_params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_ENTITY_PARAMS_H_
#define SRC_ARENA_ENTITY_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <nanogui/nanogui.h>
#include "src/common.h"
#include "src/position.h"
#include "src/color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/
/**
 * @brief The parameters used in construction of any ArenaEntity.
 *
 * These parameters can be changed in a given entity after construction, but
 * the structure itself is saved for use in resetting.
 */
struct arena_entity_params {
  arena_entity_params(void) : radius(), pos(), color() {}

  /**
   * @brief Copy an existing arena_entity_params struct.
   */
  explicit arena_entity_params(const arena_entity_params* p) :
    radius(p->radius),
    pos(p->pos),
    color(p->color) {}

  /**
   * @brief The radius of the (circular) entity, in pixels.
   */
  double radius;

  /**
   * @brief The initial position of the entity,
   */
  Position pos;

  /**
   * @brief The initial color of the entity.
   */
  Color color;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_ENTITY_PARAMS_H_
