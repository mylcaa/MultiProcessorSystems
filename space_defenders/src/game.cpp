#include "game.hpp"
#include <chrono>

Game::Game(Player &player, std::vector<Asteroid> &asteroids, int width,
           int height)
    : player(player), asteroids(asteroids), width(width), height(height) {
  frame = cv::Mat(height, width, CV_8UC3);
}

void Game::run() {
  using clock = std::chrono::high_resolution_clock;
  auto last = clock::now();

  while (running) {
    auto now = clock::now();
    float dt = std::chrono::duration<float>(now - last).count();
    last = now;

    processInput();
    update(dt);
    render();

    if (cv::waitKey(1) == 27) // ESC
      running = false;
  }
}

void Game::processInput() {
  int key = cv::waitKey(1);

  if (key == 'a') {
    if(player.getPosition() >= PLAYER_STEP) {
      player.setPosition(-PLAYER_STEP);
    }
  }

  if (key == 'd') {
    if(player.getPosition() < (width - PLAYER_STEP)) {
      player.setPosition(PLAYER_STEP);
    }
  }

  if (key == ' ' || key == 'w') {
    player.fireLaser();
  }
}

void Game::update(float dt) {
  player.update(dt);

  for(Asteroid& iterator : asteroids) {
    iterator.update(dt);
  }
}

void Game::render() {
  frame.setTo(cv::Scalar(0, 0, 0));

  player.draw(frame);

  for(Asteroid& iterator : asteroids) {
    iterator.draw(frame);
  }

  cv::imshow("Space Invaders (OpenCV)", frame);
}
