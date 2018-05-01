/**
 * @file arena_immobile_entity.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_IMMOBILE_ENTITY_H_
#define SRC_ARENA_IMMOBILE_ENTITY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena_entity.h"
#include "src/color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A base class representing an immobile entity within the arena
 */
class ArenaImmobileEntity : public ArenaEntity {
 public:
  /**
   * @brief Constructs an immobile ArenaEntity with given parameters.
   *
   * @param params The parameters of the new entity (radius, position, color).
   */
  explicit ArenaImmobileEntity(const struct arena_entity_params* const params) :
      ArenaEntity(params) {}

  /**
   * @brief Returns false (since the entity is immobile).
   */
  bool is_mobile(void) const { return false; }
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_IMMOBILE_ENTITY_H_
