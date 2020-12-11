#ifndef HOME_RUN_DERBY_APP_H
#define HOME_RUN_DERBY_APP_H

#include <string>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "simulator.h"

namespace home_run_derby {

namespace visualizer {

using ci::Color;
using ci::ColorA;
using glm::vec2;
using std::string;

/**
 * Allows us to visualize the game using the backend calculations.
 */
class HomeRunDerbyApp : public ci::app::App {
 public:
  /**
   * Constructor.
   */
  HomeRunDerbyApp();

  /**
   * Draws the graphics on the canvas.
   */
  void draw() override;

  /**
   * Contains an event when the mouse is moved.
   * @param event Contains information about the mouse in the canvas.
   */
  void mouseMove(ci::app::MouseEvent event) override;

  /**
   * Contains an event when the mouse is dragged.
   * @param event Contains information about the mouse in the canvas.
   */
  void mouseDrag(ci::app::MouseEvent event) override;

  /**
   * Contains an event when a key is pressed.
   * @param event Contains information about the key pressed.
   */
  void keyDown(ci::app::KeyEvent event) override;

 private:
  /**
   * Displays the start screen before the game starts.
   */
  void DisplayStartScreen() const;

  /**
   * Displays the end screen when the game is over.
   */
  void DisplayEndScreen() const;

  /**
   * Draws the background for the game.
   */
  void DrawGameBackground() const;

  /**
   * Draws the stars on the UI.
   */
  void DrawStars() const;

  /**
   * Draws the ground on the UI.
   */
  void DrawGround() const;

  /**
   * Draws the character on the UI.
   */
  void DrawCharacter() const;

  /**
   * Draws the bat on the UI.
   */
  void DrawBat() const;

  /**
   * Draws the ball on the UI.
   */
  void DrawBall() const;

  /**
   * Displays the game statistics, e.g. outs, score.
   */
  void DisplayGameStatistics() const;

  /**
   * Draws the variable canvas features.
   */
  void DrawCanvasFeatures() const;

  /**
   * Draws a solid left at the given corners.
   * @param top_left The top left corner to fix the rectangle at.
   * @param bottom_right The bottom right corner to fix the rectangle at.
   */
  void DrawSolidRect(const vec2& top_left, const vec2& bottom_right) const;

  /**
   * Converts a float to a string with precision.
   * @param float_to_convert The float to convert into a string.
   */
  const string FloatToString(float float_to_convert) const;

  /** BEGIN CONSTANTS **/

  /** CANVAS DIMENSION CONSTANTS **/
  /** Determines the vertical window size. **/
  const float kWindowSize = 1000;
  /** Determines the screen stretch factor in the x-direction. **/
  const float kStretchConstant = 16.0f / 9.0f;
  /** Controls the frame rate of the game. **/
  const float kFrameRate = 144;

  /** UI COLOR CONSTANTS **/
  /** The color of the start screen. **/
  const Color kStartScreenColor = Color("lightblue");
  /** The color of the end screen. **/
  const Color kEndScreenColor = Color("lightblue");
  /** The color of the background. **/
  const Color kGameBackgroundColor = Color("lightblue");
  /** The color of the ground. **/
  const Color kGroundColor = Color("green");
  /** The color of the dirt. **/
  const Color kDirtColor = Color(152.0f / 255, 76.0f / 255, 25.0f / 255);
  /** The color of the dirt particles in the ground. **/
  const Color kDirtParticleColor =
      Color(223.0f / 255, 169.0f / 255, 93.0f / 255);
  /** The color of the player. **/
  const Color kPlayerColor = Color("orange");
  /** The color of the stars. **/
  const Color kStarColor = Color(1, 1, 1);

  /** UI CONSTANTS **/
  /** The number of stars to show on the canvas at a time. **/
  const size_t kNumStars = 75;
  /** The number of dirt particles to show on the canvas at a time. **/
  const size_t kNumDirtParticles = 50;
  /** The radius of the stars. **/
  const float kStarRadius = 3;
  /** The radius of the dirt particles. **/
  const float kDirtParticleRadius = 2;
  /** The height of the ground. **/
  const float kGroundHeight = 70;
  /** Controls background color change w.r.t. vertical displacement. **/
  const float kColorChangePerDist = 100000;
  /** The radius of the player's body. **/
  const float kPlayerRadius = 90;

  /** TEXT DISPLAY CONSTANTS **/
  /** The color of the text in the start screen. **/
  const Color kStartScreenTextColor = Color("black");
  /** The text font in the start screen. **/
  const string kStartScreenTextFont = "Segue";
  /** The text font size in the start screen. **/
  const float kStartScreenTextFontSize = 100;
  /** The color of the text of the game statistics. **/
  const Color kStatisticsTextColor = Color("black");
  /** The text font of the game statistics. **/
  const string kStatisticsFont = "Segue";
  /** The text font size of the game statistics. **/
  const float kStatisticsFontSize = 50;
  /** The game statistics text location. **/
  const float kStatisticsLocation = 20;
  /** Precision for decimals shown for statistics. **/
  const float kPrecision = 0;

  /** BALL CONSTANTS **/
  /** The color of the ball. **/
  const Color kBallColor = Color("white");
  /** The mass of the ball. **/
  const float kBallMass = 10;
  /** The radius of the ball. **/
  const float kBallRadius = 50;

  /** BAT CONSTANTS **/
  /** The color of the bat. **/
  const Color kBatColor = Color(152.0f / 255, 76.0f / 255, 25.0f / 255);
  /** The mass of the bat. **/
  const float kBatMass = 5;
  /** The radius of the bat. **/
  const float kBatRadius = 15;
  /** Factor limiting the furthest point on the screen the bat can go. **/
  const float kBatXLimitFactor = 3;

  /** GAME LOGIC CONSTANTS **/
  /** The maximum number of outs. **/
  const size_t kMaxOuts = 10;
  /** A scale factor for feet travelled versus pixels. **/
  const float kDistanceScaleConstant = 50;
  /** A velocity boost factor for the ball being hit. **/
  const float kBallVelocityBoostFactor = 1.5f;
  /** The gravity acting on the ball. **/
  const float kGravity = 0.09f;
  /** The amount of friction on the ground. **/
  const float kGroundFriction = 0.1f;
  /** The restitution from the ground when bouncing. **/
  const float kGroundRestitution = 0.4f;
  /** The terminal velocity of the ball in the y-direction. **/
  const float kBallTerminalVelocity = 1000;
  /** The minimum pitch speed in the x-direction. **/
  const float kMinPitchSpeedX = 13;
  /** The maximum pitch speed in the x-direction. **/
  const float kMaxPitchSpeedX = 15;
  /** The minimum pitch speed in the y-direction. **/
  const float kMinPitchSpeedY = 4;
  /** The maximum pitch speed in the y-direction. **/
  const float kMaxPitchSpeedY = 7;

  /** END CONSTANTS **/

  Simulator simulator_;
};

}  // namespace visualizer

}  // namespace home_run_derby

#endif  // HOME_RUN_DERBY_APP_H