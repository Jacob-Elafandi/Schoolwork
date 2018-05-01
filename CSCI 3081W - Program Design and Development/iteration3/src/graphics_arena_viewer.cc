/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/graphics_arena_viewer.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "src/player.h"
#include "src/home_base.h"
#include "src/obstacle.h"
#include "src/robot.h"
#include "src/arena_params.h"
#include "src/event_keypress.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
GraphicsArenaViewer::GraphicsArenaViewer(
  const struct arena_params* const params)
    : csci3081::GraphicsApp(params->x_dim, params->y_dim, "Robot Simulation"),
      arena_(new Arena(params)),
      paused_(false),
      pause_btn_(nullptr) {
  nanogui::FormHelper *gui = new nanogui::FormHelper(this);
  nanogui::ref<nanogui::Window> window = gui->addWindow(Eigen::Vector2i(10, 10),
                                                       "Simulation Controls");
  gui->addButton("Restart",
    std::bind(&GraphicsArenaViewer::OnRestartBtnPressed, this));
  pause_btn_ = gui->addButton("Pause",
    std::bind(&GraphicsArenaViewer::OnPauseBtnPressed, this));

  performLayout();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
  if (arena_->get_game_state() == kRunning) {
    last_dt += dt;
    while (last_dt > 0.05) {
      arena_->AdvanceTime();
      last_dt -= 0.05;
    }
  }
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/
void GraphicsArenaViewer::OnRestartBtnPressed() {
  arena_->Reset();
  pause_btn_->setCaption("Pause");
}

void GraphicsArenaViewer::OnPauseBtnPressed() {
  if (arena_->get_game_state() == kRunning) {
    pause_btn_->setCaption("Play");
    arena_->set_game_state(kPaused);
  } else if (arena_->get_game_state() == kPaused) {
    pause_btn_->setCaption("Pause");
    arena_->set_game_state(kRunning);
  }
}

void GraphicsArenaViewer::OnMouseMove(int x, int y) {
  std::cout << "Mouse moved to (" << x << ", " << y << ")" << std::endl;
}

void GraphicsArenaViewer::OnLeftMouseDown(int x, int y) {
  std::cout << "Left mouse button DOWN (" << x << ", " << y << ")" << std::endl;
}

void GraphicsArenaViewer::OnLeftMouseUp(int x, int y) {
  std::cout << "Left mouse button UP (" << x << ", " << y << ")" << std::endl;
}

void GraphicsArenaViewer::OnRightMouseDown(int x, int y) {
  std::cout << "Right mouse button DOWN (" << x << ", " << y << ")\n";
}

void GraphicsArenaViewer::OnRightMouseUp(int x, int y) {
  std::cout << "Right mouse button UP (" << x << ", " << y << ")" << std::endl;
}

void GraphicsArenaViewer::OnKeyDown(const char *c, int modifiers) {
  std::cout << "Key DOWN (" << c << ") modifiers=" << modifiers << std::endl;
}

void GraphicsArenaViewer::OnKeyUp(const char *c, int modifiers) {
  std::cout << "Key UP (" << c << ") modifiers=" << modifiers << std::endl;
}

void GraphicsArenaViewer::OnSpecialKeyDown(int key, int scancode,
  int modifiers) {
  if (arena_->get_game_state() == kRunning) {
    EventKeypress e(key);
    arena_->Accept(&e);
  }
  std::cout << "Special Key DOWN key=" << key << " scancode=" << scancode
            << " modifiers=" << modifiers << std::endl;
}

void GraphicsArenaViewer::OnSpecialKeyUp(int key, int scancode, int modifiers) {
  std::cout << "Special Key UP key=" << key << " scancode=" << scancode
            << " modifiers=" << modifiers << std::endl;
}

/*******************************************************************************
 * Drawing of Entities in Arena
 ******************************************************************************/
void GraphicsArenaViewer::DrawPlayer(NVGcontext *ctx,
                                     const Player* const player) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading for the player
  nvgSave(ctx);
  nvgTranslate(ctx, player->get_pos().x, player->get_pos().y);
  nvgRotate(ctx, player->get_heading_angle() * M_PI / 180);

  // player's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, player->get_radius());
  if (arena_->get_game_state() == kWon) {
    nvgFillColor(ctx, nvgRGBA(0, 255, 0, 255));  // green
  } else if (arena_->get_game_state() == kLost) {
    nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));  // red
  } else {
    nvgFillColor(ctx, nvgRGBA(player->get_color().r, player->get_color().g,
                              player->get_color().b, 255));
  }
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // player id text label
  nvgSave(ctx);
  nvgRotate(ctx, M_PI / 2.0);
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 0.0, 10.0, player->get_name().c_str(), NULL);
  nvgRestore(ctx);
  nvgRestore(ctx);
}

void GraphicsArenaViewer::DrawRobot(NVGcontext *ctx,
                                     const Robot* const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading for this robot
  nvgSave(ctx);
  nvgTranslate(ctx, robot->get_pos().x, robot->get_pos().y);
  nvgRotate(ctx, robot->get_heading_angle() * M_PI / 180);

  // robot's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, robot->get_radius());
  nvgFillColor(ctx, nvgRGBA(robot->get_color().r,
                            robot->get_color().g,
                            robot->get_color().b,
                            255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // robot id text label
  nvgSave(ctx);
  nvgRotate(ctx, M_PI / 2.0);
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 0.0, 10.0, robot->get_name().c_str(), NULL);
  nvgRestore(ctx);
  nvgRestore(ctx);
}

void GraphicsArenaViewer::DrawObstacle(NVGcontext *ctx,
                                       const Obstacle* const obstacle) {
  nvgBeginPath(ctx);
  nvgCircle(ctx, obstacle->get_pos().x, obstacle->get_pos().y,
    obstacle->get_radius());
  nvgFillColor(ctx, nvgRGBA(obstacle->get_color().r,
                            obstacle->get_color().g,
                            obstacle->get_color().b,
                            255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, obstacle->get_pos().x, obstacle->get_pos().y,
          obstacle->get_name().c_str(), NULL);
}

void GraphicsArenaViewer::DrawHomeBase(NVGcontext *ctx,
                               const HomeBase* const home) {
  nvgBeginPath(ctx);
  nvgCircle(ctx, home->get_pos().x, home->get_pos().y, home->get_radius());
  nvgFillColor(ctx, nvgRGBA(home->get_color().r,
                            home->get_color().g,
                            home->get_color().b,
                            255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, home->get_pos().x, home->get_pos().y, home->get_name().c_str(),
    NULL);
}

// this function requires an active nanovg drawing context (ctx),
// so it should probably only be called from with DrawUsingNanoVG()
void GraphicsArenaViewer::DrawRobotSensors(NVGcontext *ctx,
    const SensorProximity* const sensor, const Robot* const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered at the position and heading for this robot
  nvgSave(ctx);
  nvgTranslate(ctx, robot->get_pos().x, robot->get_pos().y);
  nvgRotate(ctx, robot->get_heading_angle() * M_PI / 180);
  double sensor_angle = sensor->get_field_of_view() * M_PI / 180;
  double sensor_dist = sensor->get_range_of_view();

  if (sensor->get_is_left()) {
    // sensor cone outline
    nvgSave(ctx);
    nvgRotate(ctx, -sensor_angle);
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, 0.0, 0.0);
    nvgLineTo(ctx, sensor_dist, 0.0);
    nvgArc(ctx, 0.0, 0.0, sensor_dist, 0.0, sensor_angle, NVG_CW);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 100));
    nvgStroke(ctx);
    nvgRestore(ctx);

    // yellow for left sensor cone
    nvgSave(ctx);
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, 0.0, 0.0);
    nvgLineTo(ctx, sensor_dist, 0.0);
    nvgArc(ctx, 0.0, 0.0, sensor_dist, 0.0, -sensor_angle, NVG_CCW);
    nvgLineTo(ctx, 0.0, 0.0);
    nvgFillColor(ctx, nvgRGBA(255, 255, 100, 150));
    nvgFill(ctx);
    nvgRestore(ctx);
  } else {
    // sensor cone outline
    nvgSave(ctx);
    nvgRotate(ctx, sensor_angle);
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, 0.0, 0.0);
    nvgLineTo(ctx, sensor_dist, 0.0);
    nvgArc(ctx, 0.0, 0.0, sensor_dist, 0.0, -sensor_angle, NVG_CCW);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 100));
    nvgStroke(ctx);
    nvgRestore(ctx);

    // blue for right sensor cone
    nvgSave(ctx);
    nvgRotate(ctx, sensor_angle);
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, 0.0, 0.0);
    nvgLineTo(ctx, sensor_dist, 0.0);
    nvgArc(ctx, 0.0, 0.0, sensor_dist, 0.0, -sensor_angle, NVG_CCW);
    nvgLineTo(ctx, 0.0, 0.0);
    nvgFillColor(ctx, nvgRGBA(100, 100, 255, 150));
    nvgFill(ctx);
    nvgRestore(ctx);
  }

  nvgRestore(ctx);
}

void GraphicsArenaViewer::DrawPlayerBattery(NVGcontext *ctx,
                                            const Player* const player) {
  nvgSave(ctx);
  double color_ratio =  player->get_battery_level() /
                        player->get_max_battery_level();
  double left_border = arena_->get_x_dim() * 4/5;
  double right_border = arena_->get_x_dim();
  double top_border = 0;
  double bottom_border = arena_->get_x_dim() * 1/15;
  double color_edge = left_border + (right_border - left_border) * color_ratio;

  // Outline
  nvgBeginPath(ctx);
  nvgMoveTo(ctx, left_border, top_border);
  nvgLineTo(ctx, left_border, bottom_border);
  nvgLineTo(ctx, right_border, bottom_border);
  nvgLineTo(ctx, right_border, top_border);
  nvgLineTo(ctx, left_border, top_border);
  nvgMoveTo(ctx, color_edge, top_border);
  nvgLineTo(ctx, color_edge, bottom_border);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 100));
  nvgStroke(ctx);

  // Fill color
  if (left_border < color_edge) {
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, left_border, top_border);
    nvgLineTo(ctx, left_border, bottom_border);
    nvgLineTo(ctx, color_edge, bottom_border);
    nvgLineTo(ctx, color_edge, top_border);
    nvgLineTo(ctx, left_border, top_border);
    if (color_ratio > 0.5) {
      // Scale from green to yellow
      nvgFillColor(ctx, nvgRGBA(
        255 + (color_ratio - 0.5) * 2 * (80 - 255),
        225 + (color_ratio - 0.5) * 2 * (220 - 225),
        53 + (color_ratio - 0.5) * 2 * (100 - 53),
        150));
    } else {
      // Scale from yellow to red
      nvgFillColor(ctx, nvgRGBA(
        194 + color_ratio * 2 * (255 - 194),
        24 + color_ratio * 2 * (225 - 24),
        7 + color_ratio * 2 * (53 - 7),
        150));
    }
    nvgFill(ctx);
  }

  // Fill black
  if (color_edge < right_border) {
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, color_edge, top_border);
    nvgLineTo(ctx, color_edge, bottom_border);
    nvgLineTo(ctx, right_border, bottom_border);
    nvgLineTo(ctx, right_border, top_border);
    nvgLineTo(ctx, color_edge, top_border);
    nvgFillColor(ctx, nvgRGBA(0, 0, 0, 150));
    nvgFill(ctx);
  }

  nvgRestore(ctx);
}

// This is the primary driver for drawing all entities in the arena.
// It is called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::DrawUsingNanoVG(NVGcontext *ctx) {
  // initialize text rendering settings
  nvgFontSize(ctx, 18.0f);
  nvgFontFace(ctx, "sans-bold");
  nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

  std::vector<Robot*> robots = arena_->get_robots();
  for (size_t i = 0; i < robots.size(); i++) {
    DrawRobot(ctx, robots[i]);
  } /* for(i..) */

  std::vector<std::tuple<SensorProximity*, Robot*>> sensor_tuples =
    arena_->get_sensor_proximity_tuples();
  for (size_t i = 0; i < sensor_tuples.size(); i++) {
    DrawRobotSensors(ctx, std::get<0>(sensor_tuples[i]),
      std::get<1>(sensor_tuples[i]));
  }

  std::vector<Obstacle*> obstacles = arena_->get_obstacles();
  for (size_t i = 0; i < obstacles.size(); i++) {
    DrawObstacle(ctx, obstacles[i]);
  } /* for(i..) */

  DrawPlayer(ctx, arena_->get_player());
  DrawHomeBase(ctx, arena_->get_home_base());
  DrawPlayerBattery(ctx, arena_->get_player());
}

NAMESPACE_END(csci3081);
