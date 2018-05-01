/**
 * @file home_base.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_HOME_BASE_H_
#define SRC_HOME_BASE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/home_base_params.h"
#include "src/arena_mobile_entity.h"
#include "src/motion_handler_home_base.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A mobile object. If the Player collides with it, the game is won.
 */
class HomeBase: public ArenaMobileEntity {
 public:
  /**
   * @brief Constructs a new HomeBase with given parameters.
   */
  explicit HomeBase(const struct home_base_params* const params);
  std::string get_name(void) const { return "Home Base"; }
  entity_types get_entity_type(void) const { return kHomeBase; }
  void Reset();
  void TimestepUpdate();

  double get_heading_angle(void) const {
    return motion_handler_->get_heading_angle(); }
  void set_heading_angle(double ha) { motion_handler_->set_heading_angle(ha); }
  double get_speed(void) const { return motion_handler_->get_speed(); }
  void set_speed(double sp) { motion_handler_->set_speed(sp); }

 private:
  const home_base_params* params_;
  MotionHandlerHomeBase * motion_handler_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_HOME_BASE_H_
