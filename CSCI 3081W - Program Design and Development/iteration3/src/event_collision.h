/**
 * @file event_collision.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_EVENT_COLLISION_H_
#define SRC_EVENT_COLLISION_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include "src/event_base_class.h"
#include "src/entity_types.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/**
 * @brief The event that an entity has collided, either with another entity
 * or with the arena wall.
 */
class EventCollision : public EventBaseClass {
 public:
  /**
   * @brief Initializes the collision as having not taken place, and inserts
   * dummy values for the point and angle of contact.
   */
  EventCollision();
  void EmitMessage(void);

  /**
   * @brief Returns true iff the collision actually took place.
   */
  bool get_collided() const { return collided_; }

  /**
   * @brief Marks the collision as having actually taken place or not.
   *
   * @param c true if the collision took place, false otherwise.
   */
  void set_collided(bool c) { collided_ = c; }

  /**
   * @brief The position at which the collision took place.
   */
  Position get_point_of_contact() const { return point_of_contact_; }

  /**
   * @brief Assigns a position to the collision.
   *
   * @param p The position at which the collision took place.
   */
  void set_point_of_contact(Position p) { point_of_contact_ = p; }

  /**
   * @brief The angle at which the collision took place, relative to the
   * center of the entity accepting the collision, clockwise from "east" in
   * degrees (0-359).
   */
  double get_angle_of_contact() const { return angle_of_contact_; }

  /**
   * @brief Assigns an angle of contact to the collision.
   *
   * @param aoc The angle at which the collision took place, relative to the
   * center of the entity accepting the collision, clockwise from "east" in
   * degrees (0-359).
   */
  void set_angle_of_contact(double aoc) { angle_of_contact_ = aoc; }

  /**
   * @brief Marks the collision as being with a specific type of entity
   * (e.g. robot, obstacle, wall).
   */
  void set_entity_type(entity_types et) { type_of_entity_ = et; }

  /**
   * @brief Return the type of entity that has been collided with.
   */
  entity_types get_entity_type(void) const { return type_of_entity_; }

 private:
  // If collided_ is false, the collision shouldn't be accepted.
  bool collided_;
  Position point_of_contact_;
  // angle_of_contact_ is measured from the center of the ArenaEntity to the
  // point of contact, in degrees clockwise from east (0-359).
  double angle_of_contact_;
  entity_types type_of_entity_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_EVENT_COLLISION_H_
