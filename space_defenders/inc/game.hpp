#ifndef GAME_HPP
#define GAME_HPP

#include <opencv2/opencv.hpp>
#include <vector>

#include "asteroid.hpp"
#include "player.hpp"

#define WINNING_SCORE 30
#define FRAME_WIDTH 800
#define FRAME_HEIGHT 600

class Game final {
private:
  int width;
  int height;
  int activeAsteroids = 2;
  int score = 0;
  bool running = true;
  bool won = false;
  bool lost = false;

  Player &player;
  std::vector<Asteroid> &asteroids;
  cv::Mat frame;

public:
  Game(Player &player, std::vector<Asteroid> &asteroids, int width, int height);
  void run();

private:
  /* 1st thread - handle key input, player movement and laser firing */
  void processInput();

  /* (n-2) threads - update active asteroids and laser position */
  void update(float dt, float gameDuration);

  /* 2nd thread - update GUI */
  void render();
};

#endif
