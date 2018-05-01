/**
 * @file arena_entity.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_ENTITY_H_
#define SRC_ARENA_ENTITY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <nanogui/nanogui.h>
#include <string>
#include <vector>
#include "src/common.h"
#include "src/position.h"
#include "src/color.h"
#include "src/arena_entity_params.h"
#include "src/entity_types.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A base class representing an entity within the arena.
 *
 * All entities within the arena know how to :
 *
 * 1. Update themselves each timestep (i.e. in accordance with current velocity
 *    and position).
 * 2. Reset themselves to a newly constructed state. This is so the user can
 *    click the reset button after completing a game and have things work as
 *    expected.
 *
 * Note that not all classes need to be able to do these two things.
 */
class ArenaEntity {
 public:
  /**
   * @brief Constructs an ArenaEntity with given radius, position, and color.
   *
   * @param params The parameters of the new entity (radius, position, color)
   */
  explicit ArenaEntity(const struct arena_entity_params* const params) :
    params_(params), sensors_(), radius_(params->radius),
    pos_(params->pos), color_(params->color) {}

  virtual ~ArenaEntity(void) {}

  /**
   * @brief Perform whatever updates are needed for a particular entity after
   * a single timestep (updating position, changing color, etc.).
   */
  virtual void TimestepUpdate() {}

  /**
   * @brief Reset the entity to its newly constructed state.
   */
  virtual void Reset() {
    radius_ = params_->radius;
    pos_ = params_->pos;
    color_ = params_->color;
  }

  /**
   * @brief Returns the name of the entity.
   */
  virtual std::string get_name(void) const = 0;

  /**
   * @brief Returns the params of the entity.
   */
  const arena_entity_params* get_params(void) const { return params_; }

  /**
   * @brief Moves the entity to a new position.
   *
   * @param pos The position to be moved to
   */
  void set_pos(const Position& pos) { pos_ = pos; }

  /**
   * @brief Returns the entity's current position.
   */
  const Position& get_pos(void) const { return pos_; }

  /**
   * @brief Paints the entity a different color.
   *
   * @param color The color to paint the entity.
   */
  void set_color(const Color& color) { color_ = color; }

  /**
   * @brief Returns the entity's current color.
   */
  const Color& get_color(void) const { return color_; }

  /**
   * @brief Returns True for mobile entities and False for immobile entities.
   */
  virtual bool is_mobile(void) const = 0;

  /**
   * @brief Returns the radius of the entity.
   */
  double get_radius(void) const { return radius_; }

  /**
   * @brief Returns a list of the entity's sensors, if any.
   */
  std::vector<class Sensor*> get_sensors(void) const { return sensors_; }

  /**
   * @brief Returns the type of the entity.
   */
  virtual entity_types get_entity_type(void) const = 0;

 protected:
  /**
   * @brief The parameters used in construction, which are referenced when
   * resetting the entity to its newly constructed state with Reset().
   */
  const arena_entity_params* params_;

  /**
   * @brief A list of the entity's sensors, if any.
   */
  std::vector<class Sensor*> sensors_;

 private:
  double radius_;
  Position pos_;
  Color color_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_ENTITY_H_
