/**
 * @file event_keypress.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_EVENT_KEYPRESS_H_
#define SRC_EVENT_KEYPRESS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "src/event_base_class.h"
#include "src/event_commands.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief The event that the user has pressed a key on the keyboard.
 */
class EventKeypress : public EventBaseClass {
 public:
  /**
   * @brief Constructs an event holding a keypress and stores the unique
   * numerical value of the key that was pressed.
   *
   * @param key The number representing the key that was pressed.
   */
  explicit EventKeypress(int key) : key_(key) {}

  void EmitMessage(void) { printf("Keypress command received\n"); }

  /**
   * @brief Returns the number representing the key that was pressed.
   */
  int get_key(void) const {return key_;}

  /**
   * @brief Returns the command that the keypress translates to.
   *
   * Up arrow means speed up.
   * Down arrow means slow down.
   * Left arrow means turn counterclockwise.
   * Right arrow means turn clockwise.
   */
  enum event_commands get_key_cmd() const;

 private:
  int key_;

  EventKeypress& operator=(const EventKeypress& other) = delete;
  EventKeypress(const EventKeypress& other) = delete;
};

NAMESPACE_END(csci3081);

#endif  // SRC_EVENT_KEYPRESS_H_
