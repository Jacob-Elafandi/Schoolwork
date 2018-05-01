/**
 * @file entity_types.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */

#ifndef SRC_ENTITY_TYPES_H_
#define SRC_ENTITY_TYPES_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/
/**
 * @brief The types of entities in the arena.
 */
enum entity_types {
  kNone,
  kRobot,
  kSuperBot,
  kPlayer,
  kHomeBase,
  kRechargeStation,
  kObstacle,
  kWall
};
NAMESPACE_END(csci3081);

#endif  // SRC_ENTITY_TYPES_H_
