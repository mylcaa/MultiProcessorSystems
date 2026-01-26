#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <opencv2/opencv.hpp>
#include <vector>

#include "player.hpp"
#include "asteroid.hpp"

class Game final {
private:
  int width;
  int height;
  bool running = true;

  Player& player;
  std::vector<Asteroid>& asteroids;
  cv::Mat frame;

public:
  Game(Player& player, std::vector<Asteroid>& asteroids, int width, int height);

  void run();

private:
  /* Update player position and handle firing of laser */
  void processInput();
  /* Update active asteroid and laser position */
  void update(float dt);
  void render();
};

#endif
