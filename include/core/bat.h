#ifndef HOME_RUN_DERBY_BAT_H
#define HOME_RUN_DERBY_BAT_H

#include "cinder/gl/gl.h"

namespace home_run_derby {

using glm::vec2;

/**
 * Stores information about the bat which the user controls with their mouse.
 */
class Bat {
 public:
  /**
   * Default constructor.
   */
  Bat() = default;

  Bat(float bat_mass, float bat_radius);

  void SetBatSpeed(const vec2& new_speed);

  void SetBatPosition(const vec2& new_position);

  float GetBatMass() const;

  float GetBatRadius() const;

  const vec2& GetBatSpeed() const;

  const vec2& GetBatPosition() const;

 private:
  float bat_mass_;
  float bat_radius_;
  vec2 bat_speed_;
  vec2 bat_position_;
};

}  // namespace home_run_derby

#endif  // HOME_RUN_DERBY_BAT_H
