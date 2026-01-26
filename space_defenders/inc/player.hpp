#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"
#include "laser.hpp"
#include <vector>

#define PLAYER_STEP 20

class Player final : public Entity {
private:
  float speed = 300.0f;
  std::vector<Laser> &lasers;

public:
  Player(cv::Point &&pos, cv::Size &&size, std::vector<Laser> &lasers);
  ~Player() = default;

  void update(float dt) override;
  void draw(cv::Mat &frame) override;

  int getPosition();
  void setPosition(int dx);
  void fireLaser();
};

#endif