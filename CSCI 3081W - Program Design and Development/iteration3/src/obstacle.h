/**
 * @file obstacle.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_OBSTACLE_H_
#define SRC_OBSTACLE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/arena_immobile_entity.h"
#include "src/color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief An immobile entity that other entities bounce off of.
 */
class Obstacle: public ArenaImmobileEntity {
 public:
  /**
   * @brief Calls the constructor for ArenaImmobileEntity and gives
   * the newly constructed Obstacle a unique ID.
   */
  explicit Obstacle(const struct arena_entity_params* const params);

  std::string get_name(void) const {
    return "Obstacle" + std::to_string(id_);
  }
  entity_types get_entity_type(void) const { return kObstacle; }

 private:
  static uint next_id_;

  int id_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_OBSTACLE_H_
