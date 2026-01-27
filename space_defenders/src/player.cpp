#include "player.hpp"

Player::Player(cv::Point &&pos, cv::Size &&size, std::vector<Laser> &lasers)
    : Entity(std::move(pos), std::move(size)), lasers(lasers) {}

void Player::update(float dt) {
  for (Laser &iterator : lasers) {
    iterator.update(dt);
  }
}

void Player::draw(cv::Mat &frame) {
  cv::Point bottomRight(pos.x + size.width, pos.y + size.height);
  cv::rectangle(frame, pos, bottomRight, {0, 255, 0}, -1);

  for (Laser &iterator : lasers) {
    if (iterator.getActiveStatus() == false)
      continue;

    iterator.draw(frame);
  }
}

int Player::getPosition() { return pos.x; }

void Player::setPosition(int dx) { pos.x += dx; }

void Player::fireLaser() {

  for (Laser &iterator : lasers) {
    if (iterator.getActiveStatus() == false) {
      iterator.setActiveStatus(true);
      iterator.setPosition(cv::Point(pos.x + size.width / 2 - 2, pos.y));

      break;
    }
  }
}

bool Player::hitAsteroid(cv::Point asteroidPosition, cv::Size asteroidSize) {
  cv::Point laserPosition;

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
      return true;
    }
  }

  return false;
}