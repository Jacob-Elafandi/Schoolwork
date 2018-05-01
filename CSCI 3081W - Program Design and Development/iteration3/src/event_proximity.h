/**
 * @file event_proximity.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_EVENT_PROXIMITY_H_
#define SRC_EVENT_PROXIMITY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include "src/event_base_class.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/**
 * @brief The event that an entity is in the range of a proximity sensor.
 */
class EventProximity : public EventBaseClass {
 public:
  /**
   * @brief Initializes the proximity detection as not actually occurring, and
   * inserts a dummy value for its distance.
   */
  EventProximity();
  void EmitMessage(void);

  /**
   * @brief Returns true iff there's actually an entity in view.
   */
  bool get_entity_in_view() const { return entity_in_view_; }

  /**
   * @brief Marks the entity as actually being in view or not.
   *
   * @param c true if entity is emitting a distress call, false otherwise
   */
  void set_entity_in_view(bool c) { entity_in_view_ = c; }

  /**
   * @brief Returns the distance to the detected entity.
   */
  double get_distance_to_entity() const { return distance_to_entity_; }

  /**
   * @brief Marks the distance to the detected entity.
   *
   * @param dte The distance to the detected entity.
   */
  void set_distance_to_entity(double dte) { distance_to_entity_ = dte; }

  /**
   * @brief Returns the position of the detected entity.
   */
  Position get_position_of_entity() const { return position_of_entity_; }

  /**
   * @brief Marks the position of the detected entity.
   *
   * @param poe The position of the detected entity.
   */
  void set_position_of_entity(Position poe) { position_of_entity_ = poe; }

 private:
  // If entity_in_view_ is false, the event shouldn't be accepted.
  bool entity_in_view_;
  double distance_to_entity_;
  Position position_of_entity_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_EVENT_PROXIMITY_H_
