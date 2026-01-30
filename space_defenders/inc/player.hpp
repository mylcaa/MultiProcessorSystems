#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"
#include "laser.hpp"
#include <vector>
#include <omp.h>

#define PLAYER_STEP 10
#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 40

class Player final : public Entity {
private:
  float speed = 300.0f;
  std::vector<Laser> &lasers;

  omp_lock_t player_lock;

public:
  Player(cv::Point &&pos, cv::Size &&size, std::vector<Laser> &lasers);
  ~Player();

  void update(float dt) override;
  void draw(cv::Mat &frame) override;

  int getPosition();
  void setPosition(int dx);
  void fireLaser();
  bool hitAsteroid(cv::Point asteroidPosition, cv::Size asteroidSize);
};

#endif