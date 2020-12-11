#include "core/bat.h"

namespace home_run_derby {

Bat::Bat(float bat_mass, float bat_radius)
    : bat_mass_(bat_mass), bat_radius_(bat_radius) {
}

void Bat::SetBatSpeed(const vec2& new_speed) {
  bat_speed_ = new_speed;
}

void Bat::SetBatPosition(const vec2& new_position) {
  bat_position_ = new_position;
}

float Bat::GetBatMass() const {
  return bat_mass_;
}

float Bat::GetBatRadius() const {
  return bat_radius_;
}

const vec2& Bat::GetBatSpeed() const {
  return bat_speed_;
}

const vec2& Bat::GetBatPosition() const {
  return bat_position_;
}

}  // namespace home_run_derby