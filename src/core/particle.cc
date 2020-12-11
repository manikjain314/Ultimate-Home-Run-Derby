#include "core/particle.h"

#include "cinder/Rand.h"

namespace home_run_derby {

using ci::randFloat;
using glm::vec2;

Particle::Particle(const vec2 &position, bool randomize_velocity)
    : position_(position),
      speed_multiplier_(randomize_velocity ? randFloat(kMinVelocityMultiplier,
                                                       kMaxVelocityMultiplier)
                                           : 1) {
}

void Particle::UpdatePosition(const vec2 &velocity) {
  // Use the speed dampening factor (if necessary) to update the position.
  position_ += speed_multiplier_ * velocity;
}

void Particle::SetPosition(const vec2 &new_position) {
  position_ = new_position;
}

const vec2 &Particle::GetPosition() const {
  return position_;
}

}  // namespace home_run_derby