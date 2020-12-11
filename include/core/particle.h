#ifndef HOME_RUN_DERBY_PARTICLE_H
#define HOME_RUN_DERBY_PARTICLE_H

#include "cinder/gl/gl.h"

namespace home_run_derby {

using glm::vec2;

/**
 * Describes a particle in the canvas.
 */
class Particle {
 public:
  /**
   * Default constructor.
   */
  Particle() = default;

  /**
   * Creates a particle at the specified position.
   * @param position The position on the canvas to create the particle.
   * @param randomize_velocity Determines whether or not to randomize the
   * velocity of the particle.
   */
  Particle(const vec2& position, bool randomize_velocity = true);

  /**
   * Updates the position with a specified velocity.
   * @param velocity The velocity to update with respect to.
   */
  void UpdatePosition(const vec2& velocity);

  void SetPosition(const vec2& new_position);

  const vec2& GetPosition() const;

 private:
  const float kMinVelocityMultiplier = 0.1f;
  const float kMaxVelocityMultiplier = 0.4f;

  float speed_multiplier_;
  vec2 position_;
};

}  // namespace home_run_derby

#endif  // HOME_RUN_DERBY_PARTICLE_H