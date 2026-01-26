#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "entity.hpp"

class Asteroid final : public Entity {
private:
  float speed = 300;
  int frameHeight;

public:
  Asteroid(cv::Size &&size, int frameHeight);
  void update(float dt) override;
  void draw(cv::Mat &frame) override;
  ~Asteroid() = default;
};

#endif