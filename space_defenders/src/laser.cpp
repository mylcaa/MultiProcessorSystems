#include "laser.hpp"

Laser::Laser(cv::Size &&size) : Entity(std::move(size)) { alive = false; }

void Laser::update(float dt) {
  std::cout << "LASER alive = " << alive << " dt = " << dt << std::endl;
  std::cout << "BEFORE pos.y = " << pos.y << std::endl;
  if (alive == false) {
    return;
  }
  
  pos.y -= speed * dt;
  std::cout << "AFTER pos.y = " << pos.y << std::endl;
  if (pos.y < 0) {
    alive = false;
  }
  std::cout << "AFTER alive = " << alive << std::endl;
}

void Laser::draw(cv::Mat &frame) {
  cv::Point bottomRight(pos.x + size.width, pos.y + size.height);
  cv::rectangle(frame, pos, bottomRight, {255, 255, 255}, -1);
}

void Laser::setPosition(cv::Point &&newPosition) {
  pos = std::move(newPosition);
}

cv::Point Laser::getPosition() { return pos; }

cv::Size Laser::getSize() { return size; }

void Laser::setActiveStatus(bool status) { alive = status; }
