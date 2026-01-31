#include "player.hpp"

Player::Player(cv::Point &&pos, cv::Size &&size, std::vector<Laser> &lasers)
    : Entity(std::move(pos), std::move(size)), 
      lasers(lasers) 
    {
      omp_init_lock(&player_lock);
      omp_init_lock(&laser_lock);
    }

Player::~Player() { 
  omp_destroy_lock(&player_lock);
  omp_destroy_lock(&laser_lock);
}

void Player::update(float dt) {
  omp_set_lock(&laser_lock);
  for (Laser &iterator : lasers) {
    iterator.update(dt);
  }
  omp_unset_lock(&laser_lock);
}

void Player::draw(cv::Mat &frame) {

  omp_set_lock(&player_lock);
  cv::Point bottomRight(pos.x + size.width, pos.y + size.height);
  cv::rectangle(frame, pos, bottomRight, {0, 255, 0}, -1);
  omp_unset_lock(&player_lock);

  omp_set_lock(&laser_lock);
  for (Laser &iterator : lasers) {
    if (iterator.getActiveStatus() == false) {
      continue;
    }
    iterator.draw(frame);
  }
  omp_unset_lock(&laser_lock);
}

void Player::fireLaser() {
  omp_set_lock(&laser_lock);
  for (Laser &iterator : lasers) {
    if (iterator.getActiveStatus() == false) {
      iterator.setActiveStatus(true);
      iterator.setPosition(cv::Point(pos.x + size.width / 2 - 2, pos.y));
      break;
    }
  }
  omp_unset_lock(&laser_lock);
}

bool Player::hitAsteroid(cv::Point asteroidPosition, cv::Size asteroidSize) {
  cv::Point laserPosition;

  omp_set_lock(&laser_lock);
  for (Laser &iterator : lasers) {
    // skip inactive lasers
    if (iterator.getActiveStatus() == false) {
      continue;
    }

    laserPosition = iterator.getPosition();

    if ((laserPosition.x >= asteroidPosition.x) &&
        ((laserPosition.x + LASER_WIDTH) <=
         (asteroidPosition.x + asteroidSize.width)) &&
        (laserPosition.y <= (asteroidPosition.y + asteroidSize.height))) {
      iterator.setActiveStatus(false);
      omp_unset_lock(&laser_lock);
      return true;
    }
  }
  omp_unset_lock(&laser_lock);

  return false;
}

int Player::getPosition() { 
  omp_set_lock(&player_lock);
  int x = pos.x; 
  omp_unset_lock(&player_lock);
  return x;
}

void Player::setPosition(int dx) { 
  omp_set_lock(&player_lock);
  pos.x += dx; 
  omp_unset_lock(&player_lock);
}
