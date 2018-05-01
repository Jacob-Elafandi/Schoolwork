/**
 * @file player_params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_PLAYER_PARAMS_H_
#define SRC_PLAYER_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena_mobile_entity_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/

/**
 * @brief The parameters used in construction of a player.
 */
struct player_params : public arena_mobile_entity_params {
  player_params(void) :
    arena_mobile_entity_params(),
    max_speed(),
    battery_max_charge(),
    angle_delta(),
    speed_delta(),
    max_frozen_timer() {}

  /**
   * @brief The maximum speed, in pixels per timestep, that the player can
   * be accelerated to by keypresses.
   */
  double max_speed;

  /**
   * @brief The maximum level that the battery can be charged to.
   */
  double battery_max_charge;

  /**
   * @brief The number of degrees that the player will turn by when commanded
   * to do so by user keypress.
   */
  uint angle_delta;

  /**
  * @brief The change in speed, in pixels per timestep, that the
  * player undertakes when sped up or slowed down by user keypress.
  */
  double speed_delta;

  /**
   * @brief When Player collides with a SuperBot, it will be immobile for
   * this number of frames.
   */
  uint max_frozen_timer;
};

NAMESPACE_END(csci3081);

#endif  // SRC_PLAYER_PARAMS_H_
