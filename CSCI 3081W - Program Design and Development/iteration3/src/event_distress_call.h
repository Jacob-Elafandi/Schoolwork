/**
 * @file event_distress_call.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_EVENT_DISTRESS_CALL_H_
#define SRC_EVENT_DISTRESS_CALL_H_

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
 * @brief The event that an entity is outputting a distress call.
 */
class EventDistressCall : public EventBaseClass {
 public:
  /**
   * @brief Initializes the distress call as having not taken place, and inserts
   * a dummy value for the point of the call.
   */
  EventDistressCall();
  void EmitMessage(void);

  /**
   * @brief Returns true iff the entity is actually emitting a distress call.
   */
  bool get_calling() const { return calling_; }

  /**
   * @brief Marks the entity as actually emitting a distress call or not.
   *
   * @param c true if entity is emitting a distress call, false otherwise
   */
  void set_calling(bool c) { calling_ = c; }

  /**
   * @brief The position of the entity outputting the distress call.
   */
  Position get_position_of_distress_call() const {
    return position_of_distress_call_; }

  /**
   * @brief Assigns a position to the source of the distress call.
   *
   * @param p The source of the distress call.
   */
  void set_point_of_distress_call(Position p) {
    position_of_distress_call_ = p; }

 private:
  // If calling_ is false, the distress call shouldn't be accepted.
  bool calling_;
  Position position_of_distress_call_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_EVENT_DISTRESS_CALL_H_
