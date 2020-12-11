#include "core/canvas_frame.h"

#include "cinder/Rand.h"

namespace home_run_derby {

using ci::randFloat;
using std::make_pair;
using std::pair;

CanvasFrame::CanvasFrame(float player_radius, float window_size,
                         float stretch_constant, float ground_height,
                         size_t num_stars, size_t num_dirt_particles,
                         float star_radius, float dirt_particle_radius)
    : player_radius_(player_radius),
      window_size_(window_size),
      stretch_constant_(stretch_constant),
      ground_height_(ground_height),
      num_stars_(num_stars),
      num_dirt_particles_(num_dirt_particles),
      star_radius_(star_radius),
      dirt_particle_radius_(dirt_particle_radius) {
  ResetState();
  PopulateStars();
  PopulateDirtParticles();
}

void CanvasFrame::PopulateStars() {
  stars_.clear();
  // Initialize the stars vector with random positions on the canvas.
  for (size_t i = 0; i < num_stars_; ++i) {
    stars_.emplace_back(vec2(randFloat(0, stretch_constant_ * window_size_),
                             randFloat(0, window_size_)));
  }
}

void CanvasFrame::PopulateDirtParticles() {
  dirt_particles_.clear();
  // Same thing as stars, except we are not randomizing the particle velocities.
  for (size_t i = 0; i < num_dirt_particles_; ++i) {
    dirt_particles_.emplace_back(
        vec2(
            randFloat(-dirt_particle_radius_,
                      dirt_particle_radius_ + window_size_ * stretch_constant_),
            window_size_ + randFloat(0, window_size_ / 2)),
        false);
  }
}

void CanvasFrame::CalculateCharacterHeadLocation(const vec2& offset) {
  player_head_location_.x =
      offset.x + window_size_ * stretch_constant_ - 2 * player_radius_;
  player_head_location_.y =
      offset.y + window_size_ - ground_height_ - player_radius_;
}

void CanvasFrame::CalculateCharacterBodyLocation(const vec2& offset) {
  player_body_location_.x =
      offset.x + window_size_ * stretch_constant_ - 2 * player_radius_;
  player_body_location_.y =
      offset.y + window_size_ - ground_height_ - 2.5f * player_radius_;
}

void CanvasFrame::CalculateGroundLocation(const vec2& offset) {
  // The ground's x offset should not be changing.
  ground_location_ = make_pair(
      vec2(0, offset.y + window_size_ - ground_height_),
      vec2(window_size_ * stretch_constant_, offset.y + window_size_));
}

void CanvasFrame::CalculateDirtLocation(const vec2& offset) {
  dirt_location_ =
      make_pair(vec2(0, offset.y + window_size_),
                vec2(window_size_ * stretch_constant_, window_size_));
}

void CanvasFrame::UpdateStarPositions(const vec2& velocity) {
  // Update the positions of all the stars, then reassign positions if particles
  // are out of canvas.
  for (Particle& star : stars_) {
    star.UpdatePosition(velocity);

    if (velocity.x > 0 && star.GetPosition().x >
                              window_size_ * stretch_constant_ + star_radius_) {
      star.SetPosition(
          vec2(-star_radius_, randFloat(0, window_size_ + star_radius_)));
    }

    if (velocity.x < 0 && star.GetPosition().x < -star_radius_) {
      star.SetPosition(vec2(window_size_ * stretch_constant_ + star_radius_,
                            randFloat(0, window_size_ + star_radius_)));
    }

    if (velocity.y > 0 && star.GetPosition().y > window_size_ + star_radius_) {
      star.SetPosition(
          vec2(randFloat(0, window_size_ * stretch_constant_ + star_radius_),
               -star_radius_));
    }

    if (velocity.y < 0 && star.GetPosition().y < -star_radius_) {
      star.SetPosition(
          vec2(randFloat(0, window_size_ * stretch_constant_ + star_radius_),
               window_size_ + star_radius_));
    }
  }
}

void CanvasFrame::UpdateDirtParticlePositions(const vec2& velocity) {
  for (Particle& dirt_particle : dirt_particles_) {
    // To avoid drifting, stop updating the y after the y velocity is below a
    // certain threshold.
    dirt_particle.UpdatePosition(
        vec2(velocity.x,
             abs(velocity.y) < kVelocityConsideredStopped ? 0 : velocity.y));

    // Handle wrapping around.
    if (dirt_particle.GetPosition().x >
        window_size_ * stretch_constant_ + dirt_particle_radius_) {
      dirt_particle.SetPosition(
          vec2(-dirt_particle_radius_,
               window_size_ + randFloat(0, window_size_ / 2) + offset_.y));
    }
  }
}

void CanvasFrame::UpdateCanvas(const vec2& offset, const vec2& velocity) {
  offset_ = offset;
  CalculateCharacterHeadLocation(offset);
  CalculateCharacterBodyLocation(offset);
  CalculateGroundLocation(offset);
  CalculateDirtLocation(offset);
  UpdateStarPositions(velocity);
  UpdateDirtParticlePositions(velocity);
}

void CanvasFrame::ResetState() {
  UpdateCanvas(vec2(0, 0), vec2(0, 0));
  PopulateStars();
  PopulateDirtParticles();
}

const vec2& CanvasFrame::GetPlayerHeadLocation() const {
  return player_head_location_;
}

const vec2& CanvasFrame::GetPlayerBodyLocation() const {
  return player_body_location_;
}

const pair<vec2, vec2>& CanvasFrame::GetGroundLocation() const {
  return ground_location_;
}

const pair<vec2, vec2>& CanvasFrame::GetDirtLocation() const {
  return dirt_location_;
}

const vector<Particle>& CanvasFrame::GetStars() const {
  return stars_;
}

const vector<Particle>& CanvasFrame::GetDirtParticles() const {
  return dirt_particles_;
}

const vec2& CanvasFrame::GetOffset() const {
  return offset_;
}

}  // namespace home_run_derby