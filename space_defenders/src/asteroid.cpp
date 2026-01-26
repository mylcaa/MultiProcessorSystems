#include "asteroid.hpp"

Asteroid::Asteroid(cv::Size &&size, int frameHeight) : Entity(std::move(size)), frameHeight(frameHeight) {}

void Asteroid::update(float dt) {
  pos.y += speed * dt;
  if (pos.y > frameHeight)
    alive = false;
}

void Asteroid::draw(cv::Mat &frame) {
  cv::Point bottomRight(pos.x + size.width, pos.y + size.height);
  cv::rectangle(frame, pos, bottomRight, {0, 0, 255}, -1);
}