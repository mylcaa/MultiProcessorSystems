#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "entity.hpp"
#include <omp.h>

#define ASTEROID_CAPACITY 6
#define ASTEROID_WIDTH 20
#define ASTEROID_HEIGHT 20

class Asteroid final : public Entity {
private:
  float speed = 100;
  int frameHeight;

public:
  Asteroid(cv::Size &&size, int frameHeight);
  ~Asteroid()=default;

  void update(float dt) override;
  void draw(cv::Mat &frame) override;

  cv::Point getPosition();
  void setPosition(int newPosition);
  cv::Size getSize();
  void setActiveStatus(bool status);
};

#endif