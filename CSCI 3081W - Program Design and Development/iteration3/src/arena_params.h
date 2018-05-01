/**
 * @file arena_params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */

#ifndef SRC_ARENA_PARAMS_H_
#define SRC_ARENA_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/player_params.h"
#include "src/home_base_params.h"
#include "src/robot_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constant Definitions
 ******************************************************************************/
#define MAX_OBSTACLES 8
#define MAX_ROBOTS 4

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/

/**
 * @brief Contains the parameters for every ArenaEntity, as well as the number
 * of obstacles and the dimensions of the arena.
 */
struct arena_params {
  arena_params(void) :
    player(),
    recharge_station(),
    home_base(),
    obstacles(),
    robots(),
    n_obstacles(),
    n_robots(),
    x_dim(),
    y_dim() {}

  /**
   * @brief The parameters used to construct the Player.
   */
  struct player_params player;
  /**
   * @brief The parameters used to construct the RechargeStation.
   */
  struct arena_entity_params recharge_station;
  /**
   * @brief The parameters used to construct the HomeBase.
   */
  struct home_base_params home_base;
  /**
   * @brief A list of arena_entity_params structs, each used to construct
   * an Obstacle.
   */
  struct arena_entity_params obstacles[MAX_OBSTACLES];
  /**
   * @brief A list of robot_params structs, each used to construct a Robot.
   */
  struct robot_params robots[MAX_ROBOTS];
  /**
   * @brief The number of Obstacles to be constructed in the arena.
   */
  size_t n_obstacles;
  /**
   * @brief The number of Robots to be constructed in the arena.
   */
  size_t n_robots;
  /**
   * @brief The width of the arena in pixels.
   */
  uint x_dim;
  /**
   * @brief The height of the arena in pixels.
   */
  uint y_dim;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_PARAMS_H_
