#ifndef LASER_HPP
#define LASER_HPP

#include "entity.hpp"

#define LASER_CAPACITY 50
#define LASER_WIDTH 4
#define LASER_HEIGHT 10

class Laser final : public Entity {
private:
  float speed = 500;

public:
  Laser(cv::Size &&size);
  ~Laser() = default;

  void update(float dt) override;
  void draw(cv::Mat &frame) override;

  void setPosition(cv::Point &&newPosition);
  cv::Point getPosition();
  cv::Size getSize();
  void setActiveStatus(bool status);
};

#endif