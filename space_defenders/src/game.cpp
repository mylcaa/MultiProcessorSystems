#include "game.hpp"
#include <chrono>
#include <random>

std::random_device dev;
std::mt19937 rng(dev());
static int distUpperLimit = FRAME_WIDTH - ASTEROID_WIDTH - 1;
std::uniform_int_distribution<std::mt19937::result_type> dist(0,
                                                              distUpperLimit);

Game::Game(Player &player, std::vector<Asteroid> &asteroids, int width,
           int height)
    : player(player), asteroids(asteroids), width(width), height(height) {
  frame = cv::Mat(height, width, CV_8UC3);
}

void Game::run() {
  using clock = std::chrono::high_resolution_clock;
  auto gameStart = clock::now();
  auto last = gameStart;

  // activate asteroids
  for (int i = 0; i < activeAsteroids; ++i) {
    asteroids.at(i).setActiveStatus(true);
    asteroids.at(i).setPosition(dist(rng));
  }

  while (running) {
    auto now = clock::now();
    float dt = std::chrono::duration<float>(now - last).count();
    float gameDuration = std::chrono::duration<float>(now - gameStart).count();
    last = now;

    processInput();
    update(dt, gameDuration);
    render();

    // X Button on frame pressed
    if (cv::getWindowProperty("Space Invaders", cv::WND_PROP_VISIBLE) < 1) {
      running = false;
    }
  }
}

void Game::processInput() {
  int key = cv::waitKey(1);

  // Esc button pressed
  if (key == 27) {
    running = false;
  }

  if (key == 'a') {
    if (player.getPosition() >= PLAYER_STEP) {
      player.setPosition(-PLAYER_STEP);
    }
  }

  if (key == 'd') {
    if (player.getPosition() < (width - PLAYER_STEP)) {
      player.setPosition(PLAYER_STEP);
    }
  }

  if (key == ' ' || key == 'w') {
    player.fireLaser();
  }
}

void Game::update(float dt, float gameDuration) {
  player.update(dt);

  for (Asteroid &iterator : asteroids) {
    // update position
    iterator.update(dt);

    // check for collision
    if (player.hitAsteroid(iterator.getPosition(), iterator.getSize())) {
      score++;
      if (score == WINNING_SCORE) {
        won = true;
        running = false;
      }

      // regenerate asteroid
      iterator.setPosition(dist(rng));
    }

    // check for asteroid landing
    if (iterator.getPosition().y >= height) {
      lost = true;
      running = false;
    }
  }
}

void Game::render() {
  frame.setTo(cv::Scalar(0, 0, 0));

  player.draw(frame);

  int i = 0;
  for (Asteroid &iterator : asteroids) {
    std::cout << i << std::endl;
    if (iterator.getActiveStatus() == false) {
      continue;
    }

    iterator.draw(frame);
  }

  if (won || lost) {
    std::string text = won ? "YOU WON" : "GAME OVER";

    int font = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 2.0;
    int thickness = 4;

    int baseline = 0;
    cv::Size textSize =
        cv::getTextSize(text, font, scale, thickness, &baseline);

    cv::Point textOrg((width - textSize.width) / 2,
                      (height + textSize.height) / 2);

    cv::Scalar color = won ? cv::Scalar(0, 255, 0)  // green
                           : cv::Scalar(0, 0, 255); // red

    cv::putText(frame, text, textOrg, font, scale, color, thickness,
                cv::LINE_AA);
  }

  cv::imshow("Space Invaders", frame);
  if (running == false) {
    cv::waitKey(0);
  }
}