/**
 * @file home_base_params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_HOME_BASE_PARAMS_H_
#define SRC_HOME_BASE_PARAMS_H_

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
 * @brief The parameters used in construction of the home base.
 */
struct home_base_params : public arena_mobile_entity_params {
  home_base_params(void) :
      arena_mobile_entity_params(),
      heading_change_odds() {}

  /**
   * @brief The percentage chance that the Home Base will change direction at
   * any given timestep.
   */
  double heading_change_odds;
};

NAMESPACE_END(csci3081);

#endif  // SRC_HOME_BASE_PARAMS_H_
