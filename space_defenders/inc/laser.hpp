#ifndef LASER_HPP
#define LASER_HPP

#include "entity.hpp"

class Laser final : public Entity {
private:
  float speed = 500;

public:
  Laser(cv::Size &&size);
  ~Laser() = default;

  void update(float dt) override;
  void draw(cv::Mat &frame) override;

  void setPosition(cv::Point&& newPosition);
  void setActiveStatus(bool status);
};

#endif