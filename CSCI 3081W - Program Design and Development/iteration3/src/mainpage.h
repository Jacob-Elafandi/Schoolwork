/**
 * @file mainpage.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MAINPAGE_H_
#define SRC_MAINPAGE_H_

/*! \mainpage CSCI3081 Robot Simulator Project
 *
 * \section intro_sec Introduction
 *
 * This software creates a 2D Arena that contains a Player, a HomeBase, a
 * RechargeStation, and a number of Obstacles and Robots. The Player and all
 * Robots travel in a straight line, but bounce off other entities at the angle
 * of incidence.
 *
 * The player has a battery that constantly depletes, but is recharged by
 * colliding with the recharge station. The arrow keys make it change speed
 * and direction.
 *
 * Robots freeze upon contact with the Player, but unfreeze upon contact with
 * another Robot. If a Robot touches the HomeBase, it becomes a SuperBot,
 * which freezes the player on contact. Robots have proximity, distress, and
 * entity type sensors that let them avoid other entities unless hitting them
 * is desirable.
 *
 * The goal of the game is to freeze all regular Robots before the battery
 * depletes or all Robots are turned into SuperBots.
 *
 * The Arena is displayed by the GraphicsArenaViewer, which contains visible
 * buttons to pause and restart.
 */

#endif  // SRC_MAINPAGE_H_
