#include "game.hpp"
#include <chrono>
#include <random>
#include <thread>

std::random_device dev;
std::mt19937 rng(dev());
static int distUpperLimit = FRAME_WIDTH - ASTEROID_WIDTH - 1;
std::uniform_int_distribution<std::mt19937::result_type> dist(0, distUpperLimit);

Game::Game(Player &player, std::vector<Asteroid> &asteroids, int width, int height, int activeAsteroids): 
  player(player), 
  asteroids(asteroids), 
  _width(width), 
  _height(height) 
{
  _activeAsteroids = activeAsteroids < ASTEROID_CAPACITY ? activeAsteroids : ASTEROID_CAPACITY;
  _score = 0;

  frame = cv::Mat(_height, _width, CV_8UC3);
  omp_init_lock(&player_lock);
  omp_init_lock(&asteroid_lock);
  omp_init_lock(&score_lock);
  omp_init_lock(&state_lock);
  omp_init_lock(&run_lock);
}

Game::~Game() {
  omp_destroy_lock(&player_lock);
  omp_destroy_lock(&asteroid_lock);
  omp_destroy_lock(&score_lock);
  omp_destroy_lock(&state_lock);
  omp_destroy_lock(&run_lock);
}

void Game::run() {
  // place asteroids
  for (int i = 0; i < _activeAsteroids; ++i) {
    asteroids.at(i).setActiveStatus(true);
    asteroids.at(i).setPosition(dist(rng));
  }

  //run the three parts of the game on seperate threads
  int num_threads = 3;
  #pragma omp parallel num_threads(num_threads)
  {
    int tid = omp_get_thread_num();

    if (tid == 0) {
      guiLoop();
    } else 
    if (tid == 1) {
      playerLoop();
    } else {
      asteroidLoop();
    }
  }
}

void Game::playerLoop() {
  using clock = std::chrono::high_resolution_clock;
  auto last = clock::now();

  while(true) {
    //check if game is over
    omp_set_lock(&run_lock);
    bool localRunning = running;
    omp_unset_lock(&run_lock);

    if (!localRunning) {
      break;
    }

    // get time difference between two loop iterations to compute new
    // position of the player and lasers
    auto now = clock::now();
    float dt = std::chrono::duration<float>(now - last).count();
    last = now;

    //check for position or shooting update
    omp_set_lock(&player_lock);
    player_action_t localPlayerState = player_action;
    player_action = player_action_t::NONE;
    omp_unset_lock(&player_lock);

    switch(localPlayerState) {
      case player_action_t::LEFT:
        if (player.getPosition() >= PLAYER_STEP) {
          player.setPosition(-PLAYER_STEP);
        }
      break;

      case player_action_t::RIGHT:
        if (player.getPosition() < (_width - PLAYER_STEP)) {
          player.setPosition(PLAYER_STEP);
        }
      break;
      
      case player_action_t::SHOOT:
        player.fireLaser();
      break;

      default:
      break;
    }

    //update player and/or laser position
    player.update(dt);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Game::asteroidLoop() {
  using clock = std::chrono::high_resolution_clock;
  auto last = clock::now();

  while(true) {
    //check if game is over
    omp_set_lock(&run_lock);
    bool localRunning = running;
    omp_unset_lock(&run_lock);

    if (!localRunning) {
      break;
    }

    // get time difference between two loop iterations to compute new
    // position of the asteroid
    auto now = clock::now();
    float dt = std::chrono::duration<float>(now - last).count();
    last = now;

//    #pragma omp parallel num_threads(_activeAsteroids)
      std::mt19937 local_rng(1234 + omp_get_thread_num());
      
//      #pragma omp for
      for (int i = 0; i < _activeAsteroids; ++i) {
        // update position
        std::cout << "asteroid[" << i << "]" << std::endl;
        omp_set_lock(&asteroid_lock);
        asteroids[i].update(dt);
        omp_unset_lock(&asteroid_lock);

        // check for collision
        omp_set_lock(&asteroid_lock);
        cv::Point position = asteroids[i].getPosition();
        cv::Size size = asteroids[i].getSize();     
        omp_unset_lock(&asteroid_lock);

        if (player.hitAsteroid(position, size)) {
          omp_set_lock(&score_lock);
          _score++;
          bool reached_score = _score == WINNING_SCORE ? true : false;
          omp_unset_lock(&score_lock);

          if (reached_score) {
            omp_set_lock(&state_lock);
            game_state.won = true;
            omp_unset_lock(&state_lock);
          }
          
          // regenerate asteroid
          omp_set_lock(&asteroid_lock);
          asteroids[i].setPosition(dist(rng));
          omp_unset_lock(&asteroid_lock);
        }

        // check for asteroid landing
        omp_set_lock(&asteroid_lock);
        int asteroidPosY = asteroids[i].getPosition().y;
        omp_unset_lock(&asteroid_lock);
        
        if (asteroidPosY >= _height) {
          omp_set_lock(&state_lock);
          game_state.lost = true;
          omp_unset_lock(&state_lock);
        }
      }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Game::guiLoop() {
  while(true) {
    //check if game is over
    omp_set_lock(&run_lock);
    bool localRunning = running;
    omp_unset_lock(&run_lock);

    if (!localRunning) {
      break;
    }

    //create window and render elements
    frame.setTo(cv::Scalar(0, 0, 0));

    drawScore();
    player.draw(frame);

    for (Asteroid &iterator : asteroids) {
      omp_set_lock(&asteroid_lock);
      if (iterator.getActiveStatus()) {
        iterator.draw(frame);
      }
      omp_unset_lock(&asteroid_lock);
    }

    //check if won or lost and draw appropriate message if that is the case
    checkEndGame();

    cv::imshow("Space Invaders", frame);

    //handle key and cursor input
    int key = cv::waitKey(1);
    processCursorInput(key);
    processKeyInput(key);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Game::processCursorInput(int key) {
    if (cv::getWindowProperty("Space Invaders", cv::WND_PROP_VISIBLE) < 1) {
      omp_set_lock(&run_lock);
      running = false;
      omp_unset_lock(&run_lock);
    }
}

void Game::processKeyInput(int key) {


    // Esc button pressed
    if (key == 27) {
      omp_set_lock(&run_lock);
      running = false;
      omp_unset_lock(&run_lock);
    }

    if (key == 'a') {
      omp_set_lock(&player_lock);
      player_action = player_action_t::LEFT;
      omp_unset_lock(&player_lock);
    }

    if (key == 'd') {
      omp_set_lock(&player_lock);
      player_action = player_action_t::RIGHT;
      omp_unset_lock(&player_lock);
    }

    if (key == ' ' || key == 'w') {
      omp_set_lock(&player_lock);
      player_action = player_action_t::SHOOT;
      omp_unset_lock(&player_lock);
    }
}

void Game::drawScore() {
    omp_set_lock(&score_lock);
    std::string scoreText = "_score " + std::to_string(_score) + "/" + std::to_string(WINNING_SCORE);
    omp_unset_lock(&score_lock);

    cv::putText(frame,
                scoreText,
                cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX,
                0.8,
                cv::Scalar(255, 255, 255),
                2,
                cv::LINE_AA);
}

void Game::checkEndGame() {
      omp_set_lock(&state_lock);
    bool result = game_state.won || game_state.lost;
    omp_unset_lock(&state_lock);

    if (result) {
      omp_set_lock(&state_lock);
      std::string text = game_state.lost ? "GAME OVER" : "YOU WON";
      omp_unset_lock(&state_lock);

      int font = cv::FONT_HERSHEY_SIMPLEX;
      double scale = 2.0;
      int thickness = 4;

      int baseline = 0;
      cv::Size textSize =
          cv::getTextSize(text, font, scale, thickness, &baseline);

      cv::Point textOrg((_width - textSize.width) / 2,
                        (_height + textSize.height) / 2);

      omp_set_lock(&state_lock);
      cv::Scalar color = game_state.lost ? cv::Scalar(0, 0, 255)
                            : cv::Scalar(0, 255, 0);
      omp_unset_lock(&state_lock);

      cv::putText(frame, text, textOrg, font, scale, color, thickness,
                  cv::LINE_AA);
          
      omp_set_lock(&run_lock);
      running = false;
      omp_unset_lock(&run_lock);
      cv::waitKey(0);
    }
}