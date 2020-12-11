#include <visualizer/simulator.h>

namespace home_run_derby {

namespace visualizer {

Simulator::Simulator(float player_radius, float window_size,
                     float stretch_constant, float ground_height,
                     float ball_mass, float ball_radius, float gravity,
                     float friction, float restitution,
                     float ball_speed_boost_factor, float terminal_velocity,
                     float min_pitch_speed_x, float max_pitch_speed_x,
                     float min_pitch_speed_y, float max_pitch_speed_y,
                     float bat_mass, float bat_radius, size_t num_stars, size_t num_dirt_particles,
                     float star_radius, float dirt_particle_radius)
    : ball_radius_(ball_radius),
      window_size_(window_size),
      window_stretch_constant_(stretch_constant),
      canvas_frame_(player_radius, window_size, stretch_constant, ground_height,
                    num_stars, num_dirt_particles, star_radius, dirt_particle_radius),
      baseball_(ball_mass, ball_radius, gravity, friction, restitution,
                ball_speed_boost_factor, terminal_velocity, min_pitch_speed_x,
                max_pitch_speed_x, min_pitch_speed_y, max_pitch_speed_y,
                window_size),
      baseball_bat_(bat_mass, bat_radius),
      outs_(0),
      current_score_(0),
      high_score_(0),
      current_game_state_(0) {
}

void Simulator::UpdateOffset(const vec2& new_offset, const vec2& new_speed) {
  // Reset the ground location of the ball after updating the canvas with the
  // offset.
  canvas_frame_.UpdateCanvas(new_offset, new_speed);
  baseball_.SetGroundLocation(canvas_frame_.GetGroundLocation().first.y);
}

void Simulator::UpdateBallStates() {
  baseball_.UpdateStates();

  // If the baseball has already collided with the bat, change the location of
  // the canvas with respect to the ball.
  if (baseball_.HitPastScreen()) {
    canvas_frame_.UpdateCanvas(
        vec2(window_size_ * window_stretch_constant_ / 2, window_size_ / 2) -
            baseball_.GetPosition(),
        -baseball_.GetSpeed());
  } else {
    baseball_.HandleBatCollisions(baseball_bat_);

    // If the ball is past the right edge of the screen when being pitched,
    // reset the states.
    if (baseball_.GetPosition().x >=
        window_size_ * window_stretch_constant_ + ball_radius_) {
      ResetStates();
    }
  }
  // Whenever the ball stops moving, reset the states.
  if (abs(baseball_.GetSpeed().x) <= kBallConsideredStoppedVelocity) {
    ResetStates();
  }
}

void Simulator::UpdateBatStates(const vec2& new_position) {
  // Set the bat velocity based on the previous bat position.
  baseball_bat_.SetBatSpeed(new_position - baseball_bat_.GetBatPosition());
  baseball_bat_.SetBatPosition(new_position);
}

void Simulator::IncrementGameState() {
  // Update the current game state and constrain it.
  ++current_game_state_ %= kNumGameStates;
}

void Simulator::ResetStates() {
  // Increment the outs if the ball was not hit past the screen.
  if (!baseball_.HitPastScreen()) {
    ++outs_;
  } else {
    current_score_ -= baseball_.GetPosition().x;
    high_score_ = fmaxf(high_score_, current_score_);
  }
  baseball_.ResetState();
  canvas_frame_.ResetState();
}

void Simulator::ResetGame() {
  ResetStates();
  outs_ = 0;
  current_score_ = 0;
}

const vec2 Simulator::GetBallDisplayPosition() const {
  if (baseball_.HitPastScreen()) {
    return vec2(window_stretch_constant_ * window_size_ / 2, window_size_ / 2);
  }
  return baseball_.GetPosition();
}

size_t Simulator::GetCurrentGameState() const {
  return current_game_state_;
}

const CanvasFrame& Simulator::GetCanvasFrame() const {
  return canvas_frame_;
}

const Ball& Simulator::GetBall() const {
  return baseball_;
}

const Bat& Simulator::GetBat() const {
  return baseball_bat_;
}

size_t Simulator::GetOuts() const {
  return outs_;
}

float Simulator::GetScore() const {
  return current_score_;
}

float Simulator::GetHighScore() const {
  return high_score_;
}

}  // namespace visualizer

}  // namespace home_run_derby
