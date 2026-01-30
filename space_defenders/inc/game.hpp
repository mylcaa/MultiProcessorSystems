#ifndef GAME_HPP
#define GAME_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <omp.h>

#include "asteroid.hpp"
#include "player.hpp"

#define WINNING_SCORE 30
#define FRAME_WIDTH 800
#define FRAME_HEIGHT 600

class Game final {
private:
  Player &player;
  std::vector<Asteroid> &asteroids;
  // game configurations
  int _width;
  int _height;
  int _activeAsteroids;
  int _score;
  
  // game flags
  bool _running = true;
  bool _won = false;
  bool _lost = false;

  cv::Mat frame;

  omp_lock_t run_lock;
  omp_lock_t asteroid_lock;
  omp_lock_t score_lock;
  omp_lock_t result_lock;

public:
  Game(Player &player, std::vector<Asteroid> &asteroids, int width, int height, int activeAsteroids);
  ~Game();
  void run();

private:
  /* 1st thread - handle key input, player movement and laser firing */
  void processInput();

  /* (n-2) threads - update active asteroids and laser position */
  void update();

  /* 2nd thread - update GUI */
  void render();
};

#endif
