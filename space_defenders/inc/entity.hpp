#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <opencv2/opencv.hpp>

class Entity {
protected:
  cv::Point pos;
  cv::Size size;
  bool alive = true;

public:
  Entity(cv::Size &&size) : size(std::move(size)) {}

  Entity(cv::Point &&pos, cv::Size &&size)
      : pos(std::move(pos)), size(std::move(size)) {}

  bool getActiveStatus() { return alive; };

  virtual void update(float dt) = 0;
  virtual void draw(cv::Mat &frame) = 0;
  virtual ~Entity() = default;
};

#endif