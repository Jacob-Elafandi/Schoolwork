/**
 * @file arena_mobile_entity.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_MOBILE_ENTITY_H_
#define SRC_ARENA_MOBILE_ENTITY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <string>
#include "src/arena_entity.h"
#include "src/event_base_class.h"
#include "src/event_collision.h"
#include "src/color.h"
#include "src/motion_handler.h"
#include "src/motion_behavior.h"
#include "src/sensor_touch.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/**
 * @brief A base class representing a mobile entity within the arena
 */
class ArenaMobileEntity : public ArenaEntity {
 public:
  /**
   * @brief Constructs a new mobile entity with given parameters.
   *
   * @param params The arena_mobile_entity_params struct with the entity's
   * initial settings.
   */
  explicit ArenaMobileEntity(const struct arena_mobile_entity_params*
                             const params);

  virtual void Reset();

  /**
   * @brief Returns true (since the entity is mobile).
   */
  bool is_mobile(void) const { return true; }

  /**
   * @brief Returns the angle that the entity is currently travelling along,
   * clockwise from "east" in degrees (0-359).
   */
  virtual double get_heading_angle(void) const = 0;

  /**
   * @brief Assigns a new heading angle to the entity.
   *
   * @param ha The new heading angle for the entity, clockwise from "east"
   * in degrees (0-359).
   */
  virtual void set_heading_angle(double ha) = 0;

  /**
   * @brief Returns the speed of the entity, in pixels per timestep.
   */
  virtual double get_speed(void) const = 0;

  /**
   * @brief Assigns a new speed to the entity.
   *
   * @param sp The new speed of the entity, in pixels per timestep.
   */
  virtual void set_speed(double sp) = 0;

  /**
   * @brief Returns the maximum distance, in pixels, that must be between the
   * entity and another entity (or the wall) for a collision to be detected.
   */
  double get_collision_delta(void) const {
    return sensor_touch_.get_collision_delta(); }

 protected:
  /**
   * @brief The parameters used in construction of the entity. Saved so the
   * entity can be reinitialized when /ref Reset is called.
   */
  const arena_mobile_entity_params* params_;

  /**
   * @brief A pointer to the entity's motion behavior.
   */
  MotionBehavior motion_behavior_;

  /**
   * @brief A pointer to the entity's touch sensor.
   */
  SensorTouch sensor_touch_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_MOBILE_ENTITY_H_
