/**
 * @file config_file_parser.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/config_file_parser.h"
#include <iostream>
#include <fstream>
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
arena_params ConfigFileParser::Parse(int argc, char ** argv) {
  aparams_.n_robots = 0;
  aparams_.n_obstacles = 0;

  std::ifstream configfile;
  argc--; argv++;
  configfile.open(* argv);
  assert(configfile.is_open());

  std::string line;
  char command[100];

  while (getline(configfile, line)) {
    if (line.length() == 0 || line[0] == '#') continue;  // comments or blanks
    sscanf(line.c_str(), "%s ", command);
    std::string cmd = command;

    if (cmd == "arena_size") {
      int x_dim, y_dim;
      sscanf(line.c_str(), "arena_size %d %d", &x_dim, &y_dim);
      aparams_.x_dim = x_dim;
      aparams_.y_dim = y_dim;
    } else if (cmd == "home_base") {
      float radius, x, y, collision_delta, heading_angle, speed,
        heading_change_odds;
      int r, g, b;
      sscanf(line.c_str(), "home_base %f %f %f %d %d %d %f %f %f %f",
        &radius, &x, &y, &r, &g, &b,
        &collision_delta, &heading_angle, &speed, &heading_change_odds);
      aparams_.home_base.radius = radius;
      aparams_.home_base.pos = {x, y};
      aparams_.home_base.color = Color(r, g, b, 255);
      aparams_.home_base.collision_delta = collision_delta;
      aparams_.home_base.heading_angle = heading_angle;
      aparams_.home_base.speed = speed;
      aparams_.home_base.heading_change_odds = heading_change_odds;
    } else if (cmd == "player") {
      float radius, x, y, collision_delta, heading_angle, speed, max_speed,
        battery_max_charge, speed_delta;
      int r, g, b, angle_delta;
      uint max_frozen_timer;
      sscanf(line.c_str(), "player %f %f %f %d %d %d %f %f %f %f %f %d %f %u",
        &radius, &x, &y, &r, &g, &b,
        &collision_delta, &heading_angle, &speed, &max_speed,
        &battery_max_charge, &angle_delta, &speed_delta, &max_frozen_timer);
      aparams_.player.radius = radius;
      aparams_.player.pos = {x, y};
      aparams_.player.color = Color(r, g, b, 255);
      aparams_.player.collision_delta = collision_delta;
      aparams_.player.heading_angle = heading_angle;
      aparams_.player.speed = speed;
      aparams_.player.max_speed = max_speed;
      aparams_.player.battery_max_charge = battery_max_charge;
      aparams_.player.angle_delta = angle_delta;
      aparams_.player.speed_delta = speed_delta;
      aparams_.player.max_frozen_timer = max_frozen_timer;
    } else if (cmd == "recharge_station") {
      float radius, x, y;
      int r, g, b;
      sscanf(line.c_str(), "recharge_station %f %f %f %d %d %d",
        &radius, &x, &y, &r, &g, &b);
      aparams_.recharge_station.radius = radius;
      aparams_.recharge_station.pos = {x, y};
      aparams_.recharge_station.color = Color(r, g, b, 255);
    } else if (cmd == "robot") {
      float radius, x, y, collision_delta, heading_angle, speed,
        proximity_range, proximity_field, distress_range, entity_type_range;
      int r, g, b, super_r, super_g, super_b;
      sscanf(line.c_str(), "robot %f %f %f %d %d %d %d %d %d %f %f %f %f"
                           " %f %f %f",
        &radius, &x, &y, &r, &g, &b, &super_r, &super_g, &super_b,
        &collision_delta, &heading_angle, &speed, &proximity_range,
        &proximity_field, &distress_range, &entity_type_range);
      aparams_.robots[aparams_.n_robots].radius = radius;
      aparams_.robots[aparams_.n_robots].pos = {x, y};
      aparams_.robots[aparams_.n_robots].color = Color(r, g, b, 255);
      aparams_.robots[aparams_.n_robots].collision_delta = collision_delta;
      aparams_.robots[aparams_.n_robots].heading_angle = heading_angle;
      aparams_.robots[aparams_.n_robots].speed = speed;
      aparams_.robots[aparams_.n_robots].proximity_range = proximity_range;
      aparams_.robots[aparams_.n_robots].proximity_field = proximity_field;
      aparams_.robots[aparams_.n_robots].distress_range = distress_range;
      aparams_.robots[aparams_.n_robots].entity_type_range = entity_type_range;
      aparams_.robots[aparams_.n_robots].super_color = Color(super_r, super_g,
                                                             super_b, 255);
      aparams_.n_robots++;
    } else if (cmd == "obstacle") {
      float radius, x, y;
      int r, g, b;
      sscanf(line.c_str(), "obstacle %f %f %f %d %d %d",
        &radius, &x, &y, &r, &g, &b);
      aparams_.obstacles[aparams_.n_obstacles].radius = radius;
      aparams_.obstacles[aparams_.n_obstacles].pos = {x, y};
      aparams_.obstacles[aparams_.n_obstacles].color = Color(r, g, b, 255);
      aparams_.n_obstacles++;
    } else {
      std::cout << "Error: Command not found in line:\n\t" << line << std::endl;
    }
  }
  return aparams_;
}

NAMESPACE_END(csci3081);
