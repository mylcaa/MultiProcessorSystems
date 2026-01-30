#include "game.hpp"
#include <chrono>
#include <random>

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
  omp_init_lock(&run_lock);
  omp_init_lock(&asteroid_lock);
  omp_init_lock(&score_lock);
  omp_init_lock(&result_lock);
}

Game::~Game() {
  omp_destroy_lock(&run_lock);
  omp_destroy_lock(&asteroid_lock);
  omp_destroy_lock(&score_lock);
  omp_destroy_lock(&result_lock);
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
      processInput();
    } else if (tid == 1) {
      render();
    } else {
      update();
    }
  }
}

void Game::processInput() {
  while(true) {
    omp_set_lock(&run_lock);
    bool localRunning = _running;
    omp_unset_lock(&run_lock);

    if (!localRunning)
      break;

    int key = cv::waitKey(1);

    // Esc button pressed
    if (key == 27) {
      omp_set_lock(&run_lock);
      _running = false;
      break;
      omp_unset_lock(&run_lock);
    }

    if (key == 'a') {
      if (player.getPosition() >= PLAYER_STEP) {
        player.setPosition(-PLAYER_STEP);
      }
    }

    if (key == 'd') {
      if (player.getPosition() < (_width - PLAYER_STEP)) {
        player.setPosition(PLAYER_STEP);
      }
    }

    if (key == ' ' || key == 'w') {
      player.fireLaser();
    }
  }
}

void Game::update() {
  using clock = std::chrono::high_resolution_clock;
  auto last = clock::now();

  while(true) {
    omp_set_lock(&run_lock);
    bool localRunning = _running;
    omp_unset_lock(&run_lock);

    if (!localRunning)
      break;

    auto now = clock::now();
    float dt = std::chrono::duration<float>(now - last).count();
    last = now;

    player.update(dt);

    omp_set_lock(&asteroid_lock);


    #pragma omp parallel num_threads(_activeAsteroids)
    { 
      std::mt19937 local_rng(1234 + omp_get_thread_num());
      
      #pragma omp for
      for (int i = 0; i < _activeAsteroids; ++i) {
        // update position
        asteroids[i].update(dt);

        // check for collision
        if (player.hitAsteroid(asteroids[i].getPosition(), asteroids[i].getSize())) {
          omp_set_lock(&score_lock);
          _score++;
          bool reached_score = _score == WINNING_SCORE ? true : false;
          omp_unset_lock(&score_lock);

          if (reached_score) {
            omp_set_lock(&result_lock);
            _won = true;
            omp_unset_lock(&result_lock);
          }
          
          // regenerate asteroid
          asteroids[i].setPosition(dist(local_rng));
        }

        // check for asteroid landing
        if (asteroids[i].getPosition().y >= _height) {
          omp_set_lock(&result_lock);
          _lost = true;
          omp_unset_lock(&result_lock);
        }
      }
    }

    omp_unset_lock(&asteroid_lock);
  }
  
}

void Game::render() {
  while(true) {
    omp_set_lock(&run_lock);
    bool localRunning = _running;
    omp_unset_lock(&run_lock);

    if (!localRunning)
      break;

    frame.setTo(cv::Scalar(0, 0, 0));

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

    player.draw(frame);

    omp_set_lock(&asteroid_lock);
    for (Asteroid &iterator : asteroids) {
      if (iterator.getActiveStatus() == false) {
        continue;
      }

      iterator.draw(frame);
    }
    omp_unset_lock(&asteroid_lock);

    omp_set_lock(&result_lock);
    bool result = _won || _lost;
    omp_unset_lock(&result_lock);

    if (result) {
      omp_set_lock(&result_lock);
      std::string text = _lost ? "GAME OVER" : "YOU WON";
      omp_unset_lock(&result_lock);

      int font = cv::FONT_HERSHEY_SIMPLEX;
      double scale = 2.0;
      int thickness = 4;

      int baseline = 0;
      cv::Size textSize =
          cv::getTextSize(text, font, scale, thickness, &baseline);

      cv::Point textOrg((_width - textSize.width) / 2,
                        (_height + textSize.height) / 2);

      omp_set_lock(&result_lock);
      cv::Scalar color = _lost ? cv::Scalar(0, 0, 255)
                            : cv::Scalar(0, 255, 0);
      omp_unset_lock(&result_lock);

      cv::putText(frame, text, textOrg, font, scale, color, thickness,
                  cv::LINE_AA);
          
      omp_set_lock(&run_lock);
      _running = false;
      omp_unset_lock(&run_lock);
    }

    cv::imshow("Space Invaders", frame);

    // X Button on frame pressed
    if (cv::getWindowProperty("Space Invaders", cv::WND_PROP_VISIBLE) < 1) {
      omp_set_lock(&run_lock);
      _running = false;
      break;
      omp_unset_lock(&run_lock);
    }

    omp_set_lock(&run_lock);
    if (_running == false) {
      cv::waitKey(0);
    }
    omp_unset_lock(&run_lock);
  }
}