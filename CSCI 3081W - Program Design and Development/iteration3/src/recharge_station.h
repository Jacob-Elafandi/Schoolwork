/**
 * @file recharge_station.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_RECHARGE_STATION_H_
#define SRC_RECHARGE_STATION_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/obstacle.h"
#include "src/color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A unique Obstacle that recharges a Player that collides with it.
 */
class RechargeStation: public Obstacle {
 public:
  /**
   * @brief Constructs the RechargeStation with the same parameters as
   * a typical Obstacle.
   */
  explicit RechargeStation(const struct arena_entity_params* const params) :
      Obstacle(params) {}
  std::string get_name(void) const {
    return "Recharge Station";
  }
  entity_types get_entity_type(void) const { return kRechargeStation; }
};

NAMESPACE_END(csci3081);

#endif  // SRC_RECHARGE_STATION_H_
