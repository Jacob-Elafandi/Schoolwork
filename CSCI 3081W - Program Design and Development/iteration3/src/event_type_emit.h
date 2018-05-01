/**
 * @file event_type_emit.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_EVENT_TYPE_EMIT_H_
#define SRC_EVENT_TYPE_EMIT_H_

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
 * @brief The event that an entity has emitted its type.
 */
class EventTypeEmit : public EventBaseClass {
 public:
  /**
   * @brief Initializes the type detection as not actually occurring, and
   * inserts a dummy value for its position.
   */
  EventTypeEmit();
  void EmitMessage(void);

  /**
   * @brief Returns true iff there's actually an entity in view.
   */
  bool get_entity_in_range() const { return entity_in_range_; }

  /**
   * @brief Marks the entity as actually being in view or not.
   *
   * @param c true if entity is emitting a distress call, false otherwise
   */
  void set_entity_in_range(bool c) { entity_in_range_ = c; }

  /**
   * @brief Returns the type of the detected entity.
   */
  entity_types get_entity_type() const { return type_; }

  /**
   * @brief Marks the type of the detected entity.
   *
   * @param et The type of the detected entity.
   */
  void set_entity_type(entity_types et) { type_ = et; }

 private:
  // If entity_in_range_ is false, the event shouldn't be accepted.
  bool entity_in_range_;
  entity_types type_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_EVENT_TYPE_EMIT_H_
