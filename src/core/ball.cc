#include "core/ball.h"

#include <vector>

#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

using ci::randFloat;
using glm::dot;
using glm::length;
using glm::vec2;
using std::invalid_argument;
using std::min;

namespace home_run_derby {

Ball::Ball(float mass, float radius, float gravity, float friction,
           float restitution, float ball_speed_boost_factor,
           float terminal_velocity, float min_x_pitch_speed,
           float max_x_pitch_speed, float min_y_pitch_speed,
           float max_y_pitch_speed, float window_size)
    : mass_(mass),
      radius_(radius),
      gravity_(gravity),
      friction_(friction),
      restitution_(restitution),
      ball_speed_boost_factor_(ball_speed_boost_factor),
      terminal_velocity_(terminal_velocity),
      min_x_pitch_speed_(min_x_pitch_speed),
      max_x_pitch_speed_(max_x_pitch_speed),
      min_y_pitch_speed_(min_y_pitch_speed),
      max_y_pitch_speed_(max_y_pitch_speed),
      window_size_(window_size),
      has_collided_(false) {
  ResetState();
}

void Ball::HandleGroundCollisions() {
  // We only need to check for collisions with the ground. When the ball
  // collides with the ground, we have to apply friction in the x-direction and
  // restitution in the y-direction.
  if (position_.y + radius_ >= ground_location_ && speed_.y > 0) {
    speed_.x *= (1 - friction_);
    speed_.y *= -restitution_;
  }
}

void Ball::HandleBatCollisions(const Bat& bat) {
  // Bat swing start position and length.
  vec2 bat_start_pos = bat.GetBatPosition() - bat.GetBatSpeed();
  vec2 bat_pos_vector = bat.GetBatPosition() - bat_start_pos;

  // Vector from the bat's start position to the center of the ball.
  vec2 bat_to_ball = position_ - bat_start_pos;

  // Projection and orthogonal components of ball to position segment vectors.
  vec2 proj_onto_pos =
      (dot(bat_pos_vector, bat_to_ball) / dot(bat_pos_vector, bat_pos_vector)) *
      (bat_pos_vector);
  vec2 orthog_onto_pos = bat_to_ball - proj_onto_pos;

  // Checking if bat endpoint is within or beyond the ball.
  if (!has_collided_ &&
      (length(position_ - bat.GetBatPosition()) <=
           radius_ + bat.GetBatRadius() ||
       (length(proj_onto_pos) + bat.GetBatRadius() <= length(bat_pos_vector) &&
        length(orthog_onto_pos) <= radius_ + bat.GetBatRadius()))) {
    has_collided_ = true;

    if (bat.GetBatSpeed().x == 0) {
      UpdateSpeedOnCollision(bat, bat.GetBatPosition() - bat.GetBatSpeed());
    } else {
      // Find the intersection between a sphere and a line.
      float slope = bat_pos_vector.y / bat_pos_vector.x;
      float intercept = bat_start_pos.y - (slope * bat_start_pos.x);
      float A = pow(slope, 2) + 1;
      float B = 2 * (slope * (intercept - position_.y) - position_.x);
      float C = pow(position_.y, 2) - pow(radius_ + bat.GetBatRadius(), 2) +
                pow(position_.x, 2) - (2 * position_.y * intercept) +
                pow(intercept, 2);
      try {
        // Use the point closest to the bat's starting position.
        pair<float, float> roots = QuadraticSolver(A, B, C);
        vec2 first_point =
            vec2(roots.first,
                 slope * (roots.first - bat_start_pos.x) + bat_start_pos.y);
        vec2 second_point =
            vec2(roots.second,
                 slope * (roots.second - bat_start_pos.x) + bat_start_pos.y);
        UpdateSpeedOnCollision(bat, length(bat_start_pos - first_point) <=
                                            length(bat_start_pos - second_point)
                                        ? first_point
                                        : second_point);
      } catch (const invalid_argument&) {
      }  // Do nothing if the quadratic has no solutions.
    }
  }
}

void Ball::UpdateSpeedOnCollision(const Bat& bat, const vec2& bat_position) {
  speed_ -= ball_speed_boost_factor_ *
            (2.0f * bat.GetBatMass() / (mass_ + bat.GetBatMass())) *
            (dot(speed_ - bat.GetBatSpeed(), position_ - bat_position) /
             (length(position_ - bat_position) *
              (length(position_ - bat_position)))) *
            (position_ - bat_position);
}

void Ball::UpdateStates() {
  // Check for collisions with the ground first. Then, update the position and
  // restrict the speed by a terminal velocity.
  HandleGroundCollisions();
  position_ += speed_;
  speed_.y = min(speed_.y + gravity_, terminal_velocity_);
}

void Ball::ResetState() {
  has_collided_ = false;
  position_.x = -radius_;
  position_.y = window_size_ / 2;
  ResetPitchVelocity();
}

void Ball::ResetPitchVelocity() {
  speed_.x = randFloat(min_x_pitch_speed_, max_x_pitch_speed_);
  speed_.y = randFloat(-max_y_pitch_speed_, -min_y_pitch_speed_);
}

const pair<float, float> Ball::QuadraticSolver(float A, float B, float C) {
  // If the discriminant is negative, then throw an exception.
  float discriminant = pow(B, 2) - 4 * A * C;
  if (discriminant < 0) {
    throw invalid_argument("No solutions to the given quadratic!");
  }
  pair<float, float> solutions;
  solutions.first = (-B + sqrt(discriminant)) / (2 * A);
  solutions.second = (-B - sqrt(discriminant)) / (2 * A);
  return solutions;
}

bool Ball::HitPastScreen() const {
  return (has_collided_ && position_.x < 0);
}

void Ball::SetGroundLocation(float ground_location) {
  ground_location_ = ground_location;
}

const vec2& Ball::GetPosition() const {
  return position_;
}

const vec2& Ball::GetSpeed() const {
  return speed_;
}

float Ball::GetRadius() const {
  return radius_;
}

}  // namespace home_run_derby