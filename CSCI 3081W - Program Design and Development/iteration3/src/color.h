/**
 * @file color.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A simplified representation of a color. Each component should be 0-255
 */
struct Color {
 public:
  /**
   * @brief Constructs a new Color with default settings of completely
   * black and opaque.
   */
  Color(void) : r(0), g(0), b(0), a(255) {}
  /**
   * @brief Constructs a new Color to be assigned to an ArenaEntity.
   *
   * @param in_r The Red component of color, from 0 to 255.
   * @param in_g The Green component of color, from 0 to 255.
   * @param in_b The Blue component of color, from 0 to 255.
   * @param in_a Opacity. 0 is transparent, 255 is completely opaque.
   */
  Color(int in_r, int in_g, int in_b, int in_a)
    : r(in_r), g(in_g), b(in_b), a(in_a) { }
  /**
   * @brief The Red component of color, from 0 to 255.
   */
  int r;
  /**
   * @brief The Green component of color, from 0 to 255.
   */
  int g;
  /**
   * @brief The Blue component of color, from 0 to 255.
   */
  int b;
  /**
   * @brief Opacity. 0 is transparent, 255 is completely opaque.
   */
  int a;
};

NAMESPACE_END(csci3081);

#endif  // SRC_COLOR_H_
