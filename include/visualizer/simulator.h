#ifndef HOME_RUN_DERBY_SIMULATOR_H
#define HOME_RUN_DERBY_SIMULATOR_H

#include <string>
#include <unordered_map>
#include <vector>

#include "cinder/gl/gl.h"
#include "core/ball.h"
#include "core/bat.h"
#include "core/canvas_frame.h"

namespace home_run_derby {

namespace visualizer {

using std::string;

/**
 * This class handles the logic behind the simulator, tying backend calculations
 * with the frontend UI.
 */
class Simulator {
 public:
  /**
   * Default constructor.
   */
  Simulator() = default;

  /**
   * Constructor to initialize the CanvasFrame object within.
   * @param player_radius The radius of the player's body.
   * @param window_size The canvas size.
   * @param stretch_constant The stretch constant for the horizontal screen
   * width.
   * @param ground_height The height of the ground.
   * @param ball_mass The mass of the baseball.
   * @param ball_radius The radius of the baseball.
   * @param gravity The gravitational force acting on the baseball.
   * @param friction The friction acting from the ground on the ball.
   * @param restitution The coefficient of restitution acting from the ground on
   * the ball.
   * @param ball_speed_boost_factor The factor of which to boost the ball's exit
   * velocity by.
   * @param terminal_velocity The maximum y-direction velocity the ball can
   * reach.
   * @param min_x_pitch_speed The minimum x-velocity of the pitch.
   * @param max_x_pitch_speed The maximum x-velocity of the pitch.
   * @param min_y_pitch_speed The minimum y-velocity of the pitch.
   * @param max_y_pitch_speed The maximum y-velocity of the pitch.
   * @param bat_mass The mass of the bat.
   * @param bat_radius The radius of the bat.
   * @param num_stars The number of stars on the canvas at a single time.
   * @param num_dirt_particles The number of dirt particles on the canvas at a
   * single time.
   * @param star_radius The radius of the stars.
   * @param dirt_particle_radius The radius of the dirt particles.
   */
  Simulator(float player_radius, float window_size, float stretch_constant,
            float ground_height, float ball_mass, float ball_radius,
            float gravity, float friction, float restitution,
            float ball_speed_boost_factor, float terminal_velocity,
            float min_pitch_speed_x, float max_pitch_speed_x,
            float min_pitch_speed_y, float max_pitch_speed_y, float bat_mass,
            float bat_radius, size_t num_stars, size_t num_dirt_particlesm,
            float star_radius, float dirt_particle_radius);

  /**
   * Updates the offset for the canvas.
   * @param new_offset The new offset for the canvas.
   * @param new_speed The new speed for the canvas.
   */
  void UpdateOffset(const vec2& new_offset = vec2(0, 0),
                    const vec2& new_speed = vec2(0, 0));

  /**
   * Updates the states of the ball.
   */
  void UpdateBallStates();

  /**
   * Updates the states of the bat.
   * @param new_position The new bat position to update to.
   */
  void UpdateBatStates(const vec2& new_position);

  /**
   * Proceeds to the next game state.
   */
  void IncrementGameState();

  /**
   * Resets the states of the game.
   */
  void ResetStates();

  /**
   * Resets the entire game.
   */
  void ResetGame();

  /**
   * Gets the location where the ball should be displayed on the canvas.
   * @return position_ if the ball has not been hit yet, center of window
   * otherwise.
   */
  const vec2 GetBallDisplayPosition() const;

  size_t GetCurrentGameState() const;

  const CanvasFrame& GetCanvasFrame() const;

  const Ball& GetBall() const;

  const Bat& GetBat() const;

  size_t GetOuts() const;

  float GetScore() const;

  float GetHighScore() const;

 private:
  // These constants should not be changed!
  const float kBallConsideredStoppedVelocity = 0.02f;
  const size_t kNumGameStates = 3;

  float ball_radius_;
  float window_size_;
  float window_stretch_constant_;

  // Game logic variables.
  size_t current_game_state_;
  size_t outs_;
  float current_score_;
  float high_score_;

  CanvasFrame canvas_frame_;
  Ball baseball_;
  Bat baseball_bat_;
};

}  // namespace visualizer

}  // namespace home_run_derby

#endif  // HOME_RUN_DERBY_SIMULATOR_H