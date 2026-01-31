#include "asteroid.hpp"

Asteroid::Asteroid(cv::Size &&size, int frameHeight)
    : Entity(std::move(size)), frameHeight(frameHeight) {
  alive = false;
}

void Asteroid::update(float dt) {
  std::cout << "ASTEROID alive = " << alive << " dt = " << dt << std::endl;
  std::cout << "BEFORE pos.y = " << pos.y << std::endl;
  if (alive == false) {
    return;
  }
  pos.y += speed * dt;
  std::cout << "AFTER pos.y = " << pos.y << std::endl;
  
}

void Asteroid::draw(cv::Mat &frame) {
  cv::Point bottomRight(pos.x + size.width, pos.y + size.height);
  cv::rectangle(frame, pos, bottomRight, {0, 0, 255}, -1);
}

cv::Point Asteroid::getPosition() { return pos; }

void Asteroid::setPosition(int newPosition) { pos = {newPosition, 0}; }

cv::Size Asteroid::getSize() { return size; }

void Asteroid::setActiveStatus(bool status) { alive = status; }