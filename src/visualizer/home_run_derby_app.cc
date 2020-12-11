#include <visualizer/home_run_derby_app.h>

#include <sstream>

namespace home_run_derby {

namespace visualizer {

using ci::ColorA;
using glm::vec2;
using std::ostringstream;

HomeRunDerbyApp::HomeRunDerbyApp()
    : simulator_(kPlayerRadius, kWindowSize, kStretchConstant, kGroundHeight,
                 kBallMass, kBallRadius, kGravity, kGroundFriction,
                 kGroundRestitution, kBallVelocityBoostFactor,
                 kBallTerminalVelocity, kMinPitchSpeedX, kMaxPitchSpeedX,
                 kMinPitchSpeedY, kMaxPitchSpeedY, kBatMass, kBatRadius,
                 kNumStars, kNumDirtParticles, kStarRadius,
                 kDirtParticleRadius) {
  ci::app::setWindowSize(static_cast<int>(kWindowSize * kStretchConstant),
                         static_cast<int>(kWindowSize));
  ci::app::setFrameRate(kFrameRate);
}

void HomeRunDerbyApp::DisplayStartScreen() const {
  ci::gl::color(kStartScreenColor);
  DrawSolidRect(vec2(0, 0), vec2(kWindowSize * kStretchConstant, kWindowSize));
  ci::gl::drawStringCentered(
      "Ultimate Home Run Derby",
      glm::vec2(kStretchConstant * kWindowSize / 2,
                kWindowSize / 2 - kStartScreenTextFontSize),
      kStartScreenTextColor,
      ci::Font(kStartScreenTextFont, kStartScreenTextFontSize));
  ci::gl::drawStringCentered(
      "Press SPACE to play",
      glm::vec2(kStretchConstant * kWindowSize / 2,
                kWindowSize / 2 + kStartScreenTextFontSize / 2),
      kStartScreenTextColor,
      ci::Font(kStartScreenTextFont, kStartScreenTextFontSize / 2));
  ci::gl::drawStringCentered(
      "High score: " +
          FloatToString(simulator_.GetHighScore() / kDistanceScaleConstant) +
          " ft.",
      glm::vec2(kStretchConstant * kWindowSize / 2,
                kWindowSize / 2 + kStartScreenTextFontSize),
      kStartScreenTextColor,
      ci::Font(kStartScreenTextFont, kStartScreenTextFontSize / 2));
}

void HomeRunDerbyApp::DisplayEndScreen() const {
  ci::gl::color(kEndScreenColor);
  DrawSolidRect(vec2(0, 0), vec2(kStretchConstant * kWindowSize, kWindowSize));
  if (simulator_.GetHighScore() == simulator_.GetScore() &&
      simulator_.GetScore() != 0) {
    ci::gl::drawStringCentered(
        "You got a new high score!",
        glm::vec2(kStretchConstant * kWindowSize / 2,
                  kWindowSize / 2 - 1 * kStartScreenTextFontSize / 8),
        kStartScreenTextColor,
        ci::Font(kStartScreenTextFont, kStartScreenTextFontSize));
  }
  ci::gl::drawStringCentered(
      "Game over!",
      glm::vec2(kStretchConstant * kWindowSize / 2,
                kWindowSize / 2 - 2 * kStartScreenTextFontSize),
      kStartScreenTextColor,
      ci::Font(kStartScreenTextFont, kStartScreenTextFontSize));
  ci::gl::drawStringCentered(
      "Total distance hit: " +
          FloatToString(simulator_.GetScore() / kDistanceScaleConstant) +
          " ft. in " + FloatToString(static_cast<float>(kMaxOuts)) + " outs",
      glm::vec2(kStretchConstant * kWindowSize / 2,
                kWindowSize / 2 - kStartScreenTextFontSize),
      kStartScreenTextColor,
      ci::Font(kStartScreenTextFont, kStartScreenTextFontSize));
  ci::gl::drawStringCentered(
      "Press SPACE to play again",
      glm::vec2(kStretchConstant * kWindowSize / 2,
                kWindowSize / 2 + kStartScreenTextFontSize),
      kStartScreenTextColor,
      ci::Font(kStartScreenTextFont, kStartScreenTextFontSize / 2));
}

void HomeRunDerbyApp::DrawGameBackground() const {
  // Draw the sky with dynamic background colors.
  ci::Color8u background_color(kGameBackgroundColor -
                               ((kWindowSize - kGroundHeight +
                                 simulator_.GetCanvasFrame().GetOffset().y) /
                                kColorChangePerDist));
  ci::gl::clear(background_color);
}

void HomeRunDerbyApp::DrawStars() const {
  // Star opacity should be dependent on the ball height.
  for (const Particle& star : simulator_.GetCanvasFrame().GetStars()) {
    ci::gl::color(ColorA(
        kStarColor,
        abs(simulator_.GetCanvasFrame().GetOffset().y / kColorChangePerDist)));
    ci::gl::drawSolidCircle(star.GetPosition(), kStarRadius);
  }
}

void HomeRunDerbyApp::DrawGround() const {
  ci::gl::color(kGroundColor);
  // Draw the grass.
  DrawSolidRect(simulator_.GetCanvasFrame().GetGroundLocation().first,
                simulator_.GetCanvasFrame().GetGroundLocation().second);
  // Draw the dirt.
  ci::gl::color(kDirtColor);
  DrawSolidRect(simulator_.GetCanvasFrame().GetDirtLocation().first,
                simulator_.GetCanvasFrame().GetDirtLocation().second);
  // Draw the dirt particles.
  ci::gl::color(kDirtParticleColor);
  for (const Particle& dirt_particle :
       simulator_.GetCanvasFrame().GetDirtParticles()) {
    ci::gl::drawSolidCircle(dirt_particle.GetPosition(), kDirtParticleRadius);
  }
}

void HomeRunDerbyApp::DrawCharacter() const {
  ci::gl::color(kPlayerColor);

  // Draw the top ellipse.
  ci::gl::drawSolidCircle(simulator_.GetCanvasFrame().GetPlayerHeadLocation(),
                          kPlayerRadius);

  // Draw the bottom ellipse.
  ci::gl::drawSolidCircle(simulator_.GetCanvasFrame().GetPlayerBodyLocation(),
                          kPlayerRadius / 2);
}

void HomeRunDerbyApp::DrawBat() const {
  // Only draw the bat if it has not collided with the ball yet.
  if (!simulator_.GetBall().HitPastScreen()) {
    ci::gl::color(kBatColor);
    ci::gl::drawSolidCircle(simulator_.GetBat().GetBatPosition(), kBatRadius);
  }
}

void HomeRunDerbyApp::DrawBall() const {
  ci::gl::color(kBallColor);
  ci::gl::drawSolidCircle(simulator_.GetBallDisplayPosition(),
                          simulator_.GetBall().GetRadius());
}

void HomeRunDerbyApp::DisplayGameStatistics() const {
  // Make the color of the statistics variable with the height of the ball.
  ci::gl::drawStringCentered(
      "Outs: " + FloatToString(static_cast<float>(simulator_.GetOuts())),
      glm::vec2(kStretchConstant * kWindowSize / 2, kStatisticsLocation),
      kStatisticsTextColor -
          simulator_.GetBall().GetPosition().y / kColorChangePerDist,
      ci::Font(kStatisticsFont, kStatisticsFontSize));
  ci::gl::drawStringCentered(
      "Total Distance: " +
          FloatToString(simulator_.GetScore() / kDistanceScaleConstant) +
          " ft.",
      glm::vec2(kStretchConstant * kWindowSize / 2,
                kStatisticsLocation + kStatisticsFontSize),
      kStatisticsTextColor -
          simulator_.GetBall().GetPosition().y / kColorChangePerDist,
      ci::Font(kStatisticsFont, kStatisticsFontSize));

  // Only draw the current distance and altitude if the ball has been hit.
  if (simulator_.GetBall().HitPastScreen()) {
    ci::gl::drawStringCentered(
        "Current Distance: " +
            FloatToString(-simulator_.GetBall().GetPosition().x /
                          kDistanceScaleConstant) +
            " ft.",
        glm::vec2(kStretchConstant * kWindowSize / 2,
                  kStatisticsLocation + 2 * kStatisticsFontSize),
        kStatisticsTextColor -
            simulator_.GetBall().GetPosition().y / kColorChangePerDist,
        ci::Font(kStatisticsFont, kStatisticsFontSize));
    ci::gl::drawStringCentered(
        "Current Altitude: " +
            FloatToString(kGroundRestitution +
                          (kWindowSize - simulator_.GetBall().GetPosition().y -
                           kGroundHeight - kBallRadius) /
                              kDistanceScaleConstant) +
            " ft.",
        glm::vec2(kStretchConstant * kWindowSize / 2,
                  kStatisticsLocation + 3 * kStatisticsFontSize),
        kStatisticsTextColor -
            simulator_.GetBall().GetPosition().y / kColorChangePerDist,
        ci::Font(kStatisticsFont, kStatisticsFontSize));
  }
}

void HomeRunDerbyApp::DrawCanvasFeatures() const {
  DrawGameBackground();
  DrawStars();
  DrawGround();
  DrawCharacter();
  DrawBall();
  DrawBat();
  DisplayGameStatistics();
}

void HomeRunDerbyApp::draw() {
  /*
   * Game states:
   *    0 = start screen,
   *    1 = in-game,
   *    2 = end screen
   */
  if (simulator_.GetCurrentGameState() == 0) {
    simulator_.ResetGame();
    DisplayStartScreen();
  } else if (simulator_.GetCurrentGameState() == 1) {
    DrawCanvasFeatures();
    simulator_.UpdateOffset();
    simulator_.UpdateBallStates();

    if (simulator_.GetOuts() >= kMaxOuts) {
      simulator_.IncrementGameState();
    }
  } else {
    DisplayEndScreen();
  }
}

void HomeRunDerbyApp::mouseMove(ci::app::MouseEvent event) {
  // Constrain how far the user's mouse can go to control the bat.
  simulator_.UpdateBatStates(
      vec2(fmaxf(static_cast<float>(event.getPos().x),
                 kWindowSize * kStretchConstant / kBatXLimitFactor),
           fminf(fmaxf(kBatRadius, static_cast<float>(event.getPos().y)),
                 kWindowSize - kBatRadius - kGroundHeight)));
}

void HomeRunDerbyApp::mouseDrag(ci::app::MouseEvent event) {
  // If the mouse is dragged, the simulator should still update the position of
  // the bat to avoid any cheap overpowered shots.
  simulator_.UpdateBatStates(vec2(fmaxf(static_cast<float>(event.getPos().x),
                                        kWindowSize * kStretchConstant / 3),
                                  event.getPos().y));
}

void HomeRunDerbyApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_SPACE:
      // If at the end or start screen and SPACE is pressed, go to the next game
      // state.
      if (simulator_.GetCurrentGameState() != 1) {
        simulator_.IncrementGameState();
      }
      break;
  }
}

void HomeRunDerbyApp::DrawSolidRect(const vec2& top_left,
                                    const vec2& bottom_right) const {
  ci::Rectf container_box(top_left, bottom_right);
  ci::gl::drawSolidRect(container_box);
}

const string HomeRunDerbyApp::FloatToString(float float_to_convert) const {
  // We are returning by value because the variable address is temporary.
  ostringstream string_stream;
  string_stream << roundf(pow(10.0f, kPrecision) * float_to_convert) /
                       pow(10.0f, kPrecision);
  return string_stream.str();
}

}  // namespace visualizer

}  // namespace home_run_derby