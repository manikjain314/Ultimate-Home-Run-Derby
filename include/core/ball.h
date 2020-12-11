#ifndef IDEAL_GAS_BALL_H
#define IDEAL_GAS_BALL_H
#include "cinder/gl/gl.h"
#include "core/bat.h"

namespace home_run_derby {

using ci::Color;
using glm::vec2;
using std::pair;

/**
 * Handles the physics and whereabouts of the baseball.
 */
class Ball {
 public:
  /**
   * Default constructor.
   */
  Ball() = default;

  /**
   * Initialize the ball with a mass, radius, gravitational force, and ground
   * location.
   * @param mass The mass of the ball.
   * @param radius The radius of the ball.
   * @param gravity The gravitational force acting on the ball.
   * @param friction The coefficient of friction acting on the ball.
   * @param restitution The coefficient of restitution for the ball's bounce.
   * relative to the canvas.
   * @param ball_speed_boost_factor The factor of which to boost the ball's exit
   * velocity by.
   * @param terminal_velocity The terminal velocity for the ball in the
   * y-direction.
   * @param min_x_pitch_speed The minimum x-velocity of the pitch.
   * @param max_x_pitch_speed The maximum x-velocity of the pitch.
   * @param min_y_pitch_speed The minimum y-velocity of the pitch.
   * @param max_y_pitch_speed The maximum y-velocity of the pitch.
   * @param window_size The size of the canvas window.
   */
  Ball(float mass, float radius, float gravity, float friction,
       float restitution, float ball_speed_boost_factor,
       float terminal_velocity, float min_x_pitch_speed,
       float max_x_pitch_speed, float min_y_pitch_speed,
       float max_y_pitch_speed, float window_size);

  /**
   * Checks and performs collisions with the ground.
   */
  void HandleGroundCollisions();

  /**
   * Collides the ball with a bat object, if in range.
   * @param bat The bat instance to collide with.
   */
  void HandleBatCollisions(const Bat& bat);

  /**
   * Updates the speed of the ball after colliding with a bat.
   * @param bat The bat instance to collide with.
   * @param bat_position The location to perform the collision at.
   */
  void UpdateSpeedOnCollision(const Bat& bat, const vec2& bat_position);

  /**
   * Updates the positions and velocities of the ball.
   */
  void UpdateStates();

  /**
   * Resets the state of the ball to what it was before it was pitched.
   */
  void ResetState();

  /**
   * Resets the initial speed of the ball when its being pitched.
   */
  void ResetPitchVelocity();

  /**
   * Computes the roots of a quadratic equation in the form ax^2 + bx + c = 0.
   * @param A The coefficient of the x^2 term.
   * @param B The coefficient of the x term.
   * @param C The coefficient of the constant term.
   * @return A pair corresponding to the solutions of the quadratic equation.
   * @throws invalid_argument if there are no solutions.
   */
  const pair<float, float> QuadraticSolver(float A, float B, float C);

  /**
   * Checks if the ball is out of the screen after collision.
   * @return true if the ball was hit past the left end of the screen, false
   * otherwise.
   */
  bool HitPastScreen() const;

  void SetGroundLocation(float ground_location);

  const vec2& GetPosition() const;

  const vec2& GetSpeed() const;

  float GetRadius() const;

 private:
  float mass_;
  float radius_;
  float gravity_;
  float ground_location_;
  float friction_;
  float restitution_;
  float ball_speed_boost_factor_;
  float terminal_velocity_;
  float min_x_pitch_speed_;
  float max_x_pitch_speed_;
  float min_y_pitch_speed_;
  float max_y_pitch_speed_;
  float window_size_;
  bool has_collided_;
  vec2 position_;
  vec2 speed_;
};

}  // namespace home_run_derby

#endif  // IDEAL_GAS_BALL_H
