#include "laser.hpp"

Laser::Laser(cv::Size &&size) : Entity(std::move(size)) { alive = false; }

void Laser::update(float dt) {
  if (alive == false) {
    return;
  }
  
  pos.y -= speed * dt;
  if (pos.y < 0) {
    alive = false;
  }
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
