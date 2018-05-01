/**
 * @file arena.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_H_
#define SRC_ARENA_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cmath>
#include <iostream>
#include <vector>
#include <tuple>
#include "src/arena_params.h"
#include "src/event_collision.h"
#include "src/event_distress_call.h"
#include "src/event_keypress.h"
#include "src/event_proximity.h"
#include "src/event_type_emit.h"
#include "src/player.h"
#include "src/robot.h"
#include "src/home_base.h"
#include "src/recharge_station.h"
#include "src/game_states.h"
#include "src/super_bot.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
struct arena_params;

/**
 * @brief The main class for the simulation of a 2D world with many robots
 * running around.
 *
 * GraphicsArenaViewer or Tests call \ref AdvanceTime to control the simulation
 * and use the get*() functions to read out the current state of the simulation
 * (i.e., the current positions and orientations of robots and obstacles).
 */
class Arena {
 public:
  /**
   * @brief Constructs the arena according to the given arena parameters.
   *
   * @param params A structure contining the dimensions of the arena, the number
   * of obstacles, and construction information for all entities.
   */
  explicit Arena(const struct arena_params * const params);

  ~Arena(void);

  /**
   * @brief Advance the simulation by a single step.
   */
  void AdvanceTime(void);

  /**
  * @brief Handle the keypress passed along by the viewer.
  *
  * @param e an event holding the key press.
  *
  */
  void Accept(EventKeypress * e);

  /**
   * @brief Reset every entity in the arena.
   */
  void Reset(void);

  /**
   * @brief Get the # of robots in the arena.
   */
  unsigned int get_n_robots(void) const { return n_robots_; }

  /**
   * @brief Get # of obstacles in the arena.
   */
  unsigned int get_n_obstacles(void) const { return n_obstacles_; }

  /**
   * @brief Get the horizontal dimension of the arena.
   */
  double get_x_dim(void) const { return x_dim_; }

  /**
   * @brief Get the vertical dimension of the arena.
   */
  double get_y_dim(void) const { return y_dim_; }

  /**
   * @brief Get a list of all robots in the arena.
   */
  std::vector<class Robot*> get_robots(void);

  /**
   * @brief Get a list of all obstacles (i.e. non-mobile entities in the arena).
   */
  std::vector<class Obstacle*> get_obstacles(void);

  /**
   * @brief Get the list of all mobile entities in the arena.
   */
  std::vector<class ArenaMobileEntity*> get_mobile_entities(void)
    { return mobile_entities_; }

  /**
   * @brief Get a list of pairs of proximity sensors and the robots they
   * belong to.
   */
  std::vector<std::tuple<class SensorProximity*, class Robot*>>
    get_sensor_proximity_tuples(void);

  /**
   * @brief Returns a pointer to the arena's solitary player.
   */
  Player* get_player(void) const { return player_; }

  /**
   * @brief Returns a pointer to the home base in the arena.
   */
  HomeBase* get_home_base(void) const { return home_base_; }

  /**
   * @brief Returns a pointer to the recharge station in the arena.
   */
  RechargeStation* get_recharge_station(void) const {
    return recharge_station_; }

  /**
   * @brief Returns the current state of the game (running, paused,
   * won, or lost).
   */
  game_states get_game_state(void) const { return state_; }

  /**
   * @brief Marks the game as running, paused, won, or lost.
   */
  void set_game_state(game_states new_state) { state_ = new_state; }

 private:
  /**
   * @brief Converts angle from degrees to radians, then takes the sine.
   */
  double DegreeSin(double angle);
  /**
   * @brief Converts angle from degrees to radians, then takes the cosine.
   */
  double DegreeCos(double angle);
  /**
   * @brief Makes an angle positive (i.e. -167 goes to 193).
   */
  double Positive(double angle);
  /**
   * @brief Calculates the distance between two entities, in pixels.
   */
  double DistanceBetweenEntities(const class ArenaEntity* const ent1,
    const class ArenaEntity* const ent2);

  /**
   * @brief Add a sensor to the list of sensors that are notified at each
   * timestep.
   */
  void RegisterSensor(Sensor * s, ArenaEntity * ent);

  /**
   * @brief Remove a sensor from the list of sensors that are notified at
   * each timestep.
   */
  void RemoveSensor(Sensor * s);

  void UpdateSensorTouch(SensorTouch * s, ArenaEntity * ent_raw);
  void UpdateSensorProximity(SensorProximity * s, ArenaEntity * ent_raw);
  void UpdateSensorDistress(SensorDistress * s, ArenaEntity * ent);
  void UpdateSensorEntityType(SensorEntityType * s, ArenaEntity * ent);

  /**
   * @brief Update all entities for a single timestep
   */
  void UpdateEntitiesTimestep(void);

  // Under certain circumstance, the compiler requires that the copy
  // constructor is not defined. This is deleting the default copy const.
  Arena& operator=(const Arena& other) = delete;
  Arena(const Arena& other) = delete;

  // Dimensions of graphics window inside which entities must operate
  double x_dim_;
  double y_dim_;
  unsigned int n_robots_;
  unsigned int n_obstacles_;

  // Status of game
  game_states state_;

  // Entities populating the arena
  Player* player_;
  RechargeStation * recharge_station_;
  HomeBase * home_base_;
  std::vector<class ArenaEntity*> entities_;
  std::vector<class ArenaMobileEntity*> mobile_entities_;
  std::vector<std::tuple<class Sensor*, class ArenaEntity*>> sensor_tuples_;
  std::vector<class Robot*> removed_robots_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_H_
