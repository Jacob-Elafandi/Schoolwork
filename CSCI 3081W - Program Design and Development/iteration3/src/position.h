/**
 * @file position.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_POSITION_H_
#define SRC_POSITION_H_

/**
 * @brief A pair of coordinates, x and y, of a point in the arena. x goes from
 * left to right, and y goes from top to bottom; both start at 0.
 */
struct Position {
  /**
   * @brief Constructs a new Position at the upper left corner of the arena.
   */
  Position(void) : x(0), y(0) { }
  /**
   * @brief Constructs a new Position at the specified coordinates.
   *
   * @param in_x Horizontal distance from the left wall, in pixels.
   * @param in_y Vertical distance from the top wall, in pixels.
   */
  Position(double in_x, double in_y) : x(in_x), y(in_y) { }
  /**
   * @brief Horizontal distance from the left wall, in pixels.
   */
  double x;
  /**
   * @brief Vertical distance from the top wall, in pixels.
   */
  double y;
};

#endif  // SRC_POSITION_H_
