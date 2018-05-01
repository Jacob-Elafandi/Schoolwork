/**
 * @file arena.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>

#include "src/arena.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Arena::Arena(const struct arena_params* const params) :
  x_dim_(params->x_dim), y_dim_(params->y_dim),
  n_robots_(params->n_robots),
  n_obstacles_(params->n_obstacles),
  state_(kRunning),
  player_(new Player(&params->player)),
  recharge_station_(new RechargeStation(&params->recharge_station)),
  home_base_(new HomeBase(&params->home_base)),
  entities_(),
  mobile_entities_(),
  sensor_tuples_(),
  removed_robots_() {
  entities_.push_back(player_);
  mobile_entities_.push_back(player_);

  entities_.push_back(recharge_station_);

  entities_.push_back(home_base_);
  mobile_entities_.push_back(home_base_);

  for (size_t i = 0; i < n_robots_; ++i) {
    Robot* robot = new Robot(&params->robots[i]);
    entities_.push_back(robot);
    mobile_entities_.push_back(robot);
  } /* for (i..) */

  for (size_t i = 0; i < n_obstacles_; ++i) {
    entities_.push_back(new Obstacle(&params->obstacles[i]));
  } /* for (i..) */

  for (auto ent : entities_) {
    for (auto s : ent->get_sensors()) {
      RegisterSensor(s, ent);
    }
  }
}

Arena::~Arena(void) {
  for (auto ent : entities_) {
    delete ent;
  } /* for (ent..) */
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Arena::RegisterSensor(Sensor * s, ArenaEntity * ent) {
  sensor_tuples_.push_back(
    std::tuple<class Sensor*, class ArenaEntity*>(s, ent));
}

void Arena::RemoveSensor(Sensor * s) {
  for (size_t i = 0; i < sensor_tuples_.size(); ++i) {
    if (std::get<0>(sensor_tuples_[i]) == s) {
      sensor_tuples_.erase(sensor_tuples_.begin() + i);
      return;
    }
  }
  std::cout << "Error: Attempted to remove unregistered sensor." << std::endl;
  assert(0);
}

void Arena::Reset(void) {
  state_ = kRunning;
  for (int j = entities_.size() - 1; j >= 0; --j) {
    ArenaEntity* ent = entities_[j];
    ent->Reset();
    if (ent->get_entity_type() == kSuperBot) {
      for (size_t i = 0; i < entities_.size(); ++i) {
        if (entities_[i] == ent) {
          entities_.erase(entities_.begin() + i);
        }
      }
      for (size_t i = 0; i < mobile_entities_.size(); ++i) {
        if (mobile_entities_[i] == ent) {
          mobile_entities_.erase(mobile_entities_.begin() + i);
        }
      }
      for (auto s : ent->get_sensors()) {
        RemoveSensor(s);
      }
      delete ent;
    }
  } /* for (ent..) */
  // Restore all robots that had been converted to SuperBots
  for (int i = removed_robots_.size() - 1; i >= 0; --i) {
    removed_robots_[i]->Reset();
    entities_.push_back(removed_robots_[i]);
    mobile_entities_.push_back(removed_robots_[i]);
    std::vector<class Sensor*> svector = removed_robots_[i]->get_sensors();
    for (auto s : svector) {
      RegisterSensor(s, removed_robots_[i]);
    }
    removed_robots_.erase(removed_robots_.begin() + i);
  }
} /* reset() */

std::vector<Obstacle*> Arena::get_obstacles(void) {
  std::vector<Obstacle*> res;
  for (auto ent : entities_) {
    Obstacle* o = dynamic_cast<Obstacle*>(ent);
    if (o) {
      res.push_back(o);
    }
  } /* for (ent..) */
  return res;
} /* get_obstacles() */

std::vector<Robot*> Arena::get_robots(void) {
  std::vector<Robot*> res;
  for (auto ent : entities_) {
    if (ent->get_entity_type() == kRobot ||
        ent->get_entity_type() == kSuperBot) {
      Robot* r = dynamic_cast<Robot*>(ent);
      assert(r);
      res.push_back(r);
    }
  } /* for (ent..) */
  return res;
} /* get_robots() */

std::vector<std::tuple<SensorProximity*, Robot*>>
    Arena::get_sensor_proximity_tuples(void) {
  std::vector<std::tuple<class SensorProximity*, class Robot*>> res;
  for (auto sens_tuple : sensor_tuples_) {
    Sensor* sens = std::get<0>(sens_tuple);
    ArenaEntity* ent = std::get<1>(sens_tuple);
    if (sens->get_sensor_type() == kSensorProximity) {
      SensorProximity* new_sens = dynamic_cast<SensorProximity*>(sens);
      Robot* new_ent = dynamic_cast<Robot*>(ent);
      if (!new_sens || !new_ent) {
        std::cout << "Error: Invalid typing of SensorProximity tuple\n";
        assert(0);
      }
      res.push_back(std::tuple<class SensorProximity*, class Robot*>(
        new_sens, new_ent));
    }
  }
  return res;
} /* get_sensor_proximity_tuples() */

void Arena::AdvanceTime(void) {
  std::cout << "Advancing simulation time by 1 timestep\n";
  UpdateEntitiesTimestep();
} /* AdvanceTime() */

void Arena::UpdateEntitiesTimestep(void) {
  /*
   * First, update the position of all entities, according to their current
   * velocities.
   */
  for (auto ent : entities_) {
    ent->TimestepUpdate();
  } /* for (ent..) */

  /*
   * Check to see if any Robots should be turned into SuperBots.
   */
  std::vector<Robot*> robots = get_robots();
  for (auto r : robots) {
    if (r->get_touched_home_base()) {
      removed_robots_.push_back(r);
      // Remove the robot from entities_ and mobile_entities_
      for (size_t i = 0; i < entities_.size(); ++i) {
        if (entities_[i] == r) {
          entities_.erase(entities_.begin() + i);
          break;
        }
      }
      for (size_t i = 0; i < mobile_entities_.size(); ++i) {
        if (mobile_entities_[i] == r) {
          mobile_entities_.erase(mobile_entities_.begin() + i);
          break;
        }
      }
      // Deregister r's sensors.
      for (auto s : r->get_sensors()) {
        RemoveSensor(s);
      }
      // Create a new SuperBot with the parameters of r.
      robot_params* sbparams = new robot_params(r->get_params());
      sbparams->color = r->get_params()->super_color;
      sbparams->pos = r->get_pos();
      sbparams->heading_angle = r->get_heading_angle();
      SuperBot* sb = new SuperBot(sbparams, r->get_id());
      entities_.push_back(sb);
      mobile_entities_.push_back(sb);
      for (auto s : sb->get_sensors()) {
        RegisterSensor(s, sb);
      }
    }
  }

  /*
   * Now check to see if all robots are frozen (in which case the game is won)
   * or if all robots have been Super-ified (in which case the game is lost).
   */
  robots = get_robots();
  bool unfrozen_robot_exists = false;
  bool non_super_robot_exists = false;
  for (auto r : robots) {
    if (r->get_entity_type() == kRobot) {
      non_super_robot_exists = true;
      if (!r->get_is_frozen()) {
        unfrozen_robot_exists = true;
        break;
      }
    }
  }
  if (!non_super_robot_exists) {
    state_ = kLost;
    std::cout << "Sorry, you lose. Press Restart to play again." << std::endl;
    return;
  }
  if (!unfrozen_robot_exists) {
    state_ = kWon;
    std::cout << "Congratulations, you win! Press Restart to play again." <<
      std::endl;
    return;
  }

  /*
   * Check if the player has run out of battery
   */
  if (player_->get_battery_level() <=0) {
    state_ = kLost;
    std::cout << "Sorry, you lose. Press Restart to play again." << std::endl;
    return;
  }

  /*
   * Now update each registered sensor, which will send the relevant signal
   * to its component entity.
   */
  for (auto sens_tuple : sensor_tuples_) {
    Sensor* sens = std::get<0>(sens_tuple);
    ArenaEntity* ent = std::get<1>(sens_tuple);
    switch (sens->get_sensor_type()) {
      case kSensorTouch:
        UpdateSensorTouch(dynamic_cast<SensorTouch*>(sens), ent);
        break;
      case kSensorProximity:
        UpdateSensorProximity(dynamic_cast<SensorProximity*>(sens), ent);
        break;
      case kSensorDistress:
        UpdateSensorDistress(dynamic_cast<SensorDistress*>(sens), ent);
        break;
      case kSensorEntityType:
        UpdateSensorEntityType(dynamic_cast<SensorEntityType*>(sens), ent);
        break;
      default:
        std::cout << "Error: Attempting to update sensor of unknown type";
        assert(0);
    }
  }
} /* UpdateEntities() */

void Arena::UpdateSensorTouch(SensorTouch * s, ArenaEntity* ent_raw) {
  assert(ent_raw->is_mobile());
  ArenaMobileEntity* ent = dynamic_cast<ArenaMobileEntity*>(ent_raw);
  EventCollision ec;

  // Check if entity is out of bounds. If so, use that as point of contact.
  if (ent->get_pos().x+ ent->get_radius() >= x_dim_
      && (ent->get_heading_angle() < 90 || ent->get_heading_angle() > 270)) {
    // Right Wall
    ec.set_collided(true);
    ec.set_entity_type(kWall);
    ec.set_point_of_contact(Position(x_dim_, ent->get_pos().y));
    ec.set_angle_of_contact(0);
  } else if (ent->get_pos().x- ent->get_radius() <= 0
      && (ent->get_heading_angle() > 90 && ent->get_heading_angle() < 270)) {
    // Left Wall
    ec.set_collided(true);
    ec.set_entity_type(kWall);
    ec.set_point_of_contact(Position(0, ent->get_pos().y));
    ec.set_angle_of_contact(180);
  } else if (ent->get_pos().y+ ent->get_radius() >= y_dim_
      && (ent->get_heading_angle() > 0 && ent->get_heading_angle() < 180)) {
    // Bottom Wall
    ec.set_collided(true);
    ec.set_entity_type(kWall);
    ec.set_point_of_contact(Position(ent->get_pos().x, y_dim_));
    ec.set_angle_of_contact(90);
  } else if (ent->get_pos().y - ent->get_radius() <= 0
      && (ent->get_heading_angle() > 180 && ent->get_heading_angle() < 360)) {
    // Top Wall
    ec.set_collided(true);
    ec.set_entity_type(kWall);
    ec.set_point_of_contact(Position(ent->get_pos().x, 0));
    ec.set_angle_of_contact(270);
  } else {
    ec.set_collided(false);
  }

  // If not at wall, check if colliding with any other entities (not itself)
  if (!ec.get_collided()) {
    for (size_t i = 0; i < entities_.size(); ++i) {
      if (entities_[i] == ent) {
        continue;
      }
      /* Note: this assumes circular entities */
      double ent1_x = ent->get_pos().x;
      double ent1_y = ent->get_pos().y;
      double ent2_x = entities_[i]->get_pos().x;
      double ent2_y = entities_[i]->get_pos().y;
      double dist = DistanceBetweenEntities(ent, entities_[i]);
      if (dist > ent->get_radius() + entities_[i]->get_radius() +
          s->get_collision_delta()) {
        ec.set_collided(false);
      } else {
        // Populate the collision event.
        // Collided is true
        // Point of contact is point along perimeter of ent1
        // Angle of contact is angle to that point of contact
        ec.set_collided(true);
        double poc_scalar = dist / (ent->get_radius() +
          entities_[i]->get_radius());
        double poc_x = ent1_x + poc_scalar * (ent2_x - ent1_x);
        double poc_y = ent1_y + poc_scalar * (ent2_y - ent1_y);
        ec.set_entity_type(kWall);
        ec.set_point_of_contact(Position(poc_x, poc_y));
        ec.set_angle_of_contact(
          fmod(std::atan2(poc_y - ent1_y, poc_x - ent1_x) * 180.0 / M_PI
          + 360.0, 360.0));
      }
      if (ec.get_collided()) {
        ec.set_entity_type(entities_[i]->get_entity_type());
        break;
      }
    } /* for (i..) */
  } /* else */
  s->Accept(&ec);
}

void Arena::UpdateSensorProximity(SensorProximity * s, ArenaEntity * ent_raw) {
  assert(ent_raw->is_mobile());
  ArenaMobileEntity* ent = dynamic_cast<ArenaMobileEntity*>(ent_raw);
  EventProximity ep;

  double sensor_lower, sensor_upper;
  double heading = ent->get_heading_angle();
  if (s->get_is_left()) {
    sensor_lower = heading - s->get_field_of_view();
    sensor_upper = heading;
  } else {
    sensor_lower = heading;
    sensor_upper = heading + s->get_field_of_view();
  }

  // Check for distance to each wall
  double wall_theta;  // Angle of shortest distance in sensor range to wall
  // Bottom Wall
  if (Positive(sensor_lower) <= 90 && Positive(sensor_upper) >= 90) {
    wall_theta = 90;
  } else if (std::abs(90 - Positive(sensor_lower)) <
      std::abs(Positive(sensor_upper) - 90)) {
    wall_theta = Positive(sensor_lower);
  } else {
    wall_theta = Positive(sensor_upper);
  }
  if (wall_theta > 0 && wall_theta < 180) {
    double dist = (y_dim_ - ent->get_pos().y) / DegreeSin(wall_theta);
    if (dist < s->get_range_of_view()) {
      ep.set_entity_in_view(true);
      ep.set_distance_to_entity(dist);
      ep.set_position_of_entity(Position(ent->get_pos().x +
                                DegreeCos(wall_theta) * dist, y_dim_));
      s->Accept(&ep);
      return;
    }
  }
  // Left Wall
  if (Positive(sensor_lower) <= 180 && Positive(sensor_upper) >= 180) {
    wall_theta = 180;
  } else if (std::abs(180 - Positive(sensor_lower)) <
      std::abs(Positive(sensor_upper) - 180)) {
    wall_theta = Positive(sensor_lower);
  } else {
    wall_theta = Positive(sensor_upper);
  }
  if (wall_theta > 90 && wall_theta < 270) {
    double dist = (ent->get_pos().x) / DegreeSin(wall_theta - 90);
    if (dist < s->get_range_of_view()) {
      ep.set_entity_in_view(true);
      ep.set_distance_to_entity(dist);
      ep.set_position_of_entity(Position(0, ent->get_pos().y +
                                   DegreeCos(wall_theta - 90) * dist));
      s->Accept(&ep);
      return;
    }
  }
  // Top Wall
  if (Positive(sensor_lower) <= 270 && Positive(sensor_upper) >= 270) {
    wall_theta = 270;
  } else if (std::abs(270 - Positive(sensor_lower)) <
      std::abs(Positive(sensor_upper) - 270)) {
    wall_theta = Positive(sensor_lower);
  } else {
    wall_theta = Positive(sensor_upper);
  }
  if (wall_theta > 180 && wall_theta < 360) {
    double dist = (ent->get_pos().y) / DegreeSin(wall_theta - 180);
    if (dist < s->get_range_of_view()) {
      ep.set_entity_in_view(true);
      ep.set_distance_to_entity(dist);
      ep.set_position_of_entity(Position(ent->get_pos().x -
                                DegreeCos(wall_theta - 180) * dist, 0));
      s->Accept(&ep);
      return;
    }
  }
  // Right Wall
  if (Positive(sensor_lower) >= Positive(sensor_upper)) {
    wall_theta = 0;
  } else if (Positive(sensor_lower) < 360 - Positive(sensor_upper)) {
    wall_theta = Positive(sensor_lower);
  } else {
    wall_theta = Positive(sensor_upper);
  }
  if (wall_theta < 90 || wall_theta > 270) {
    double dist = (x_dim_ - ent->get_pos().x) / DegreeCos(wall_theta);
    if (dist < s->get_range_of_view()) {
      ep.set_entity_in_view(true);
      ep.set_distance_to_entity(dist);
      ep.set_position_of_entity(Position(x_dim_, ent->get_pos().y +
                                        DegreeSin(wall_theta) * dist));
      s->Accept(&ep);
      return;
    }
  }

  // If not sensing a wall, check if sensing another entity.
  if (!ep.get_entity_in_view()) {
    for (size_t i = 0; i < entities_.size(); ++i) {
      if (entities_[i] == ent) {
        continue;
      }
      double dist = DistanceBetweenEntities(ent, entities_[i]);
      if (dist > s->get_range_of_view() + entities_[i]->get_radius()) {
        continue;  // Too far away
      }
      double triangle_theta = Positive(
        std::atan(entities_[i]->get_radius() / dist) * 180 / M_PI);
      double distance_theta = Positive(
        std::atan2(
          entities_[i]->get_pos().y - ent->get_pos().y,
          entities_[i]->get_pos().x - ent->get_pos().x) * 180 / M_PI);
      double sensed_lower = distance_theta - triangle_theta;
      double sensed_upper = distance_theta + triangle_theta;
      // Check overlap between sensor and sensed
      if ((sensor_lower >= sensed_lower && sensor_lower <= sensed_upper)
          || (sensor_upper >= sensed_lower && sensor_upper <= sensed_upper)) {
        ep.set_entity_in_view(true);
        ep.set_distance_to_entity(dist);
        ep.set_position_of_entity(entities_[i]->get_pos());
        break;
      }
    } /* for (i..) */
  }
  s->Accept(&ep);
}

void Arena::UpdateSensorDistress(SensorDistress * s, ArenaEntity * ent) {
  EventDistressCall ed;

  std::vector<Robot*> robots = get_robots();
  for (size_t i = 0; i < robots.size(); ++i) {
    if (robots[i] == ent) {
      continue;
    }
    if (DistanceBetweenEntities(ent, robots[i]) < s->get_range_of_view()
        && robots[i]-> get_is_frozen()) {
      ed.set_calling(true);
      ed.set_point_of_distress_call(robots[i]->get_pos());
    }
  }
  s->Accept(&ed);
}

void Arena::UpdateSensorEntityType(SensorEntityType * s, ArenaEntity * ent) {
  EventTypeEmit e;
  e.set_entity_in_range(false);
  e.set_entity_type(kNone);
  double smallest_distance = s->get_range_of_view();

  for (size_t i = 0; i < entities_.size(); ++i) {
    if (entities_[i] == ent) {
      continue;
    }
    double distance = DistanceBetweenEntities(ent, entities_[i]);
    if (distance <= smallest_distance) {
      smallest_distance = distance;
      e.set_entity_in_range(true);
      e.set_entity_type(entities_[i]->get_entity_type());
    }
  } /* for (i..) */
  if (ent->get_pos().x <= smallest_distance
      || x_dim_ - ent->get_pos().x <= smallest_distance
      || ent->get_pos().y <= smallest_distance
      || y_dim_ - ent->get_pos().y <= smallest_distance) {
    e.set_entity_in_range(true);
    e.set_entity_type(kWall);
  }
  s->Accept(&e);
}

double Arena::DegreeSin(double angle) {
  return sin(angle * M_PI / 180.0);
}

double Arena::DegreeCos(double angle) {
  return cos(angle * M_PI / 180.0);
}

double Arena::Positive(double angle) {
  return fmod(angle + 360.0, 360.0);
}

double Arena::DistanceBetweenEntities(const ArenaEntity* const ent1,
  const ArenaEntity* const ent2) {
  return std::sqrt(
    std::pow(ent2->get_pos().x - ent1->get_pos().x, 2)
    + std::pow(ent2->get_pos().y - ent1->get_pos().y, 2));
}

void Arena::Accept(EventKeypress * e) {
  EventCommand command(e->get_key_cmd());
  player_->Accept(&command);
} /* Accept(EventKeypress * e) */

NAMESPACE_END(csci3081);
