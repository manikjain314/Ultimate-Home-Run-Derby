#ifndef HOME_RUN_DERBY_CANVAS_FRAME_H
#define HOME_RUN_DERBY_CANVAS_FRAME_H
#include <vector>

#include "cinder/gl/gl.h"
#include "core/particle.h"

namespace home_run_derby {

using glm::vec2;
using std::pair;
using std::vector;

/**
 * Holds information about locations of drawings on the canvas for the current
 * location.
 */
class CanvasFrame {
 public:
  CanvasFrame() = default;

  /**
   * Constructor for parameters for drawing the canvas frame.
   * @param player_radius The radius of the player's body.
   * @param window_size The canvas size.
   * @param stretch_constant The stretch constant for the horizontal screen
   * width.
   * @param ground_height The height of the ground.
   * @param num_stars The number of stars to display on the canvas at a time.
   * @param num_dirt_particles The number of dirt particles to display on the
   * canvas at a time.
   * @param star_radius The radius of the stars.
   * @param dirt_particle_radius The radius of the dirt particles.
   */
  CanvasFrame(float player_radius, float window_size, float stretch_constant,
              float ground_height, size_t num_stars, size_t num_dirt_particles,
              float star_radius, float dirt_particle_radius);

  /**
   * Populates the vector of stars with initial values.
   */
  void PopulateStars();

  /**
   * Populates the vector of dirt particles with initial values.
   */
  void PopulateDirtParticles();

  /**
   * Calculates the head location for the character.
   * @param offset The offset to shift the canvas by.
   */
  void CalculateCharacterHeadLocation(const vec2& offset);

  /**
   * Calculates the body location for the character.
   * @param offset The offset to shift the canvas by.
   */
  void CalculateCharacterBodyLocation(const vec2& offset);

  /**
   * Calculates the location of the ground.
   * @param offset The offset to shift the canvas by.
   */
  void CalculateGroundLocation(const vec2& offset);

  /**
   * Calculates the location of the dirt below the ground.
   * @param offset The offset to shift the canvas by.
   */
  void CalculateDirtLocation(const vec2& offset);

  /**
   * Updates the positions of all the stars on the canvas.
   * @param velocity The velocity of the canvas at the time of this update call.
   */
  void UpdateStarPositions(const vec2& velocity);

  /**
   * Updates the positions of all the stars on the canvas.
   * @param velocity The velocity of the canvas at the time of the update call.
   */
  void UpdateDirtParticlePositions(const vec2& velocity);

  /**
   * Updates the canvas coordinate locations by applying an offset.
   * @param offset An offset to apply to the coordinates.
   * @param velocity The velocity of the canvas reference frame.
   */
  void UpdateCanvas(const vec2& offset, const vec2& velocity);

  /**
   * Resets the state of the canvas.
   */
  void ResetState();

  const vec2& GetPlayerHeadLocation() const;

  const vec2& GetPlayerBodyLocation() const;

  const pair<vec2, vec2>& GetGroundLocation() const;

  const pair<vec2, vec2>& GetDirtLocation() const;

  const vector<Particle>& GetStars() const;

  const vector<Particle>& GetDirtParticles() const;

  const vec2& GetOffset() const;

 private:
  // A fixed threshold when the velocity is considered "standstill."
  float kVelocityConsideredStopped = 0.5f;

  float player_radius_;
  float window_size_;
  float stretch_constant_;
  float ground_height_;
  float star_radius_;
  float dirt_particle_radius_;
  size_t num_stars_;
  size_t num_dirt_particles_;
  vec2 player_head_location_;
  vec2 player_body_location_;
  pair<vec2, vec2> ground_location_;
  pair<vec2, vec2> dirt_location_;
  vector<Particle> stars_;
  vector<Particle> dirt_particles_;
  vec2 offset_;
};
}  // namespace home_run_derby

#endif  // HOME_RUN_DERBY_CANVAS_FRAME_H
