/**
 * @file arena_mobile_entity.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/arena_mobile_entity.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
ArenaMobileEntity::ArenaMobileEntity(
    const struct arena_mobile_entity_params* const params) :
  ArenaEntity(params),
  params_(params),
  motion_behavior_(),
  sensor_touch_(params->collision_delta) {
  sensors_.push_back(&sensor_touch_);
}

void ArenaMobileEntity::Reset() {
  ArenaEntity::Reset();
  sensor_touch_.Reset();
}

NAMESPACE_END(csci3081);
