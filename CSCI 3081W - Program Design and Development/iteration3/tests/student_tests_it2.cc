
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include "../src/graphics_arena_viewer.h"  // i.e. include everything


/*******************************************************************************
 * Sensors
 ******************************************************************************/
#ifdef SENSOR_TEST

TEST(Sensor, SensorDistress) {
  csci3081::SensorDistress sens(5);
  EXPECT_NO_THROW(sens) << "FAIL: Can't reference new distress sensor";
  EXPECT_EQ(sens.get_range_of_view(), 5) << "FAIL: Range initialized incorrectly";
  EXPECT_EQ(sens.get_output(), 0) << "FAIL: Output initialized incorrectly";

  csci3081::EventDistressCall e;

  e.set_calling(true);
  sens.Accept(&e);
  EXPECT_EQ(sens.get_output(), 1) << "FAIL: Distress call does not activate sensor";

  e.set_calling(false);
  sens.Accept(&e);
  EXPECT_EQ(sens.get_output(), 0) << "FAIL: Lack of distress call does not deactivate sensor";

  e.set_calling(true);
  sens.Accept(&e);
  sens.Reset();
  EXPECT_EQ(sens.get_output(), 0) << "FAIL: Reset does not deactivate sensor";

  // delete &sens;
  // EXPECT_ANY_THROW(sens) << "FAIL: Destructor does not destroy object";
}

TEST(Sensor, SensorTouch) {
  csci3081::SensorTouch sens(0.1);
  EXPECT_NO_THROW(sens) << "FAIL: Can't reference new touch sensor";
  EXPECT_EQ(sens.get_collision_delta(), 0.1) << "FAIL: Collision delta initialized incorrectly";
  EXPECT_EQ(sens.get_output(), 0) << "FAIL: Output initialized incorrectly";

  csci3081::EventCollision e;
  e.set_angle_of_contact(45);

  e.set_collided(true);
  sens.Accept(&e);
  EXPECT_EQ(sens.get_output(), 1) << "FAIL: Collision does not activate sensor";
  EXPECT_DOUBLE_EQ(sens.get_angle_of_contact(), 45) << "FAIL: Sensor does not record angle of contact";

  e.set_collided(false);
  sens.Accept(&e);
  EXPECT_EQ(sens.get_output(), 0) << "FAIL: Lack of collision does not deactivate sensor";

  e.set_collided(true);
  sens.Accept(&e);
  sens.Reset();
  EXPECT_EQ(sens.get_output(), 0) << "FAIL: Reset does not deactivate sensor";

  // delete &sens;
  // EXPECT_ANY_THROW(sens) << "FAIL: Destructor does not destroy object";
}

TEST(Sensor, SensorEntityType) {
  csci3081::SensorEntityType sens(5);
  EXPECT_NO_THROW(sens) << "FAIL: Can't reference new entity type sensor";
  EXPECT_EQ(sens.get_range_of_view(), 5) << "FAIL: Range initialized incorrectly";
  EXPECT_EQ(sens.get_output(), csci3081::kNone) << "FAIL: Output initialized incorrectly";

  csci3081::EventTypeEmit e;
  e.set_entity_type(csci3081::kSuperBot);

  e.set_entity_in_range(true);
  sens.Accept(&e);
  EXPECT_EQ(sens.get_output(), csci3081::kSuperBot) << "FAIL: Type emission does not activate sensor correctly";

  e.set_entity_in_range(false);
  sens.Accept(&e);
  EXPECT_EQ(sens.get_output(), csci3081::kNone) << "FAIL: Lack of type emission does not deactivate sensor";

  e.set_entity_in_range(true);
  sens.Accept(&e);
  sens.Reset();
  EXPECT_EQ(sens.get_output(), csci3081::kNone) << "FAIL: Reset does not deactivate sensor";

  // delete &sens;
  // EXPECT_ANY_THROW(sens) << "FAIL: Destructor does not destroy object";
}

TEST(Sensor, SensorProximity) {
  csci3081::SensorProximity sens(5, 60, true);
  EXPECT_NO_THROW(sens) << "FAIL: Can't reference new proximity sensor";
  EXPECT_EQ(sens.get_field_of_view(), 60) << "FAIL: Field of view initialized incorrectly";
  EXPECT_EQ(sens.get_output(), -1) << "FAIL: Output initialized incorrectly";

  csci3081::EventProximity e;
  e.set_distance_to_entity(4);

  e.set_entity_in_view(true);
  sens.Accept(&e);
  EXPECT_EQ(sens.get_output(), 4) << "FAIL: Proximity event does not activate sensor";

  e.set_entity_in_view(false);
  sens.Accept(&e);
  EXPECT_EQ(sens.get_output(), -1) << "FAIL: Lack of proximity event does not deactivate sensor";

  e.set_entity_in_view(true);
  sens.Accept(&e);
  sens.Reset();
  EXPECT_EQ(sens.get_output(), -1) << "FAIL: Reset does not deactivate sensor";

  // delete &sens;
  // EXPECT_ANY_THROW(sens) << "FAIL: Destructor does not destroy object";
}

#endif /* SENSOR_TEST */

/*******************************************************************************
 * Player
 ******************************************************************************/
#ifdef PLAYER_TEST

TEST(Player, Sanity) {
  csci3081::player_params params;
  params.pos = Position(0, 0);
  params.angle_delta = 15;
  params.speed_delta = 2;
  params.battery_max_charge = 100.0;
  params.max_frozen_timer = 10;
  params.collision_delta = 0.1;
  params.radius = 5.0;
  params.speed = 4.0;
  params.max_speed = 5.0;
  csci3081::Player p(&params);
  EXPECT_EQ(p.get_name(), "Player");
  EXPECT_DOUBLE_EQ(p.radius(), 5.0) << "FAIL: Bad radius";
  EXPECT_DOUBLE_EQ(p.collision_delta(), 0.1) << "FAIL: Bad collision delta";
  EXPECT_DOUBLE_EQ(p.battery_level(), 100.0) << "FAIL: Bad battery level";
  p.speed(10.0);
  EXPECT_EQ(p.speed(), 5.0) << "FAIL: Max speed exceeded";
  bool val = std::is_base_of<csci3081::ArenaMobileEntity, csci3081::Player>::value;
  EXPECT_TRUE(val);
}

TEST(Player, Reset) {
  csci3081::player_params params;
  params.pos = Position(0, 0);
  params.angle_delta = 15;
  params.speed_delta = 2;
  params.battery_max_charge = 100.0;
  params.max_frozen_timer = 10;
  params.collision_delta = 0.1;
  params.radius = 5.0;
  params.speed = 4.0;
  params.max_speed = 5.0;
  csci3081::Player p(&params);

  p.speed(3.0);
  p.TimestepUpdate(5);

  p.Reset();
  EXPECT_DOUBLE_EQ(p.speed(), 4.0) << "FAIL: Reset speed";
  EXPECT_DOUBLE_EQ(p.battery_level(), 100.0) << "FAIL: Reset battery";
}

TEST(Player, BatteryDepletion) {
  csci3081::player_params params;
  params.pos = Position(0, 0);
  params.angle_delta = 15;
  params.speed_delta = 2;
  params.battery_max_charge = 100.0;
  params.max_frozen_timer = 10;
  params.collision_delta = 0.1;
  params.radius = 5.0;
  params.speed = 4.0;
  params.max_speed = 5.0;
  csci3081::Player p(&params);

  double level = p.battery_level()
  p.TimestepUpdate(100);

  // Test that the battery has some depletion after moving.
  EXPECT_LT(std::fabs(p.battery_level(), level) <<
      "FAIL: Battery not depleted with move";
}

TEST(Player, SuperBotFreeze) {
  csci3081::player_params params;
  params.pos = Position(0, 0);
  params.angle_delta = 15;
  params.speed_delta = 2;
  params.battery_max_charge = 100.0;
  params.max_frozen_timer = 10;
  params.collision_delta = 0.1;
  params.radius = 5.0;
  params.speed = 4.0;
  params.max_speed = 5.0;
  csci3081::Player p(&params);

  EventCollision ec;
  ec.set_collided(true);
  ec.set_entity_type(kSuperBot);
  p.sensor_touch_.Accept(ec);

  Position pos = p.get_pos();
  double heading = p.get_heading_angle();
  p.TimestepUpdate(1);
  EXPECT_DOUBLE_EQ(p.get_pos(), pos <<
    "FAIL: Player position changed after SuperBot collision");
  EXPECT_DOUBLE_EQ(p.get_heading_angle(), heading <<
    "FAIL: Player heading angle changed after SuperBot collision");
  EXPECT_TRUE(p.motion_handler_.get_is_frozen() <<
    "FAIL: Player did not freeze after SuperBot collision");

  ec.set_collided(false);
  p.TimestepUpdate(100);
  EXPECT_FALSE(p.motion_handler_.get_is_frozen() <<
    "FAIL: Player did not unfreeze after time");
}

#endif /* PLAYER_TEST */
