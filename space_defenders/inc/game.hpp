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

enum class player_action_t{
  NONE = 0,
  LEFT,
  RIGHT,
  SHOOT
};

struct game_state_t {
  bool won;
  bool lost;
};

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
  bool running = true;
  game_state_t game_state = {false, false};
  player_action_t player_action = player_action_t::NONE;

  cv::Mat frame;

  omp_lock_t run_lock;
  omp_lock_t player_lock;
  omp_lock_t asteroid_lock;
  omp_lock_t score_lock;
  omp_lock_t state_lock;

public:
  Game(Player &player, std::vector<Asteroid> &asteroids, int width, int height, int activeAsteroids);
  ~Game();
  void run();

private:
  /* 1st thread - player movement */
  void playerLoop();

  /* (n-2) threads - asteroid movement */
  void asteroidLoop();

  /* 2nd thread - handle input and render */
  void guiLoop();

  //helper functions
  void processCursorInput(int key);
  void processKeyInput(int key);
  void drawScore();
  void checkEndGame();
};

#endif
