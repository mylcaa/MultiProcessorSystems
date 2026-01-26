#include <vector>

#include "game.hpp"
#include "player.hpp"
#include "laser.hpp"
#include "asteroid.hpp"

#define LASER_CAPACITY 50
#define ASTEROID_CAPACITY 50

int main() {

//Frame dimensions
    int frameWidth = 800;
    int frameHeight = 600;

//Laser dimensions
    int laserWidth = 4;
    int laserHeight = 10;

    std::vector<Laser> lasers;
    for(int i=0; i < LASER_CAPACITY; ++i) {
        lasers.push_back(Laser(cv::Size(laserWidth, laserHeight)));
    }

//Player dimensions
    int playerWidth = 20;
    int playerHeight = 40;

    Player player(cv::Point(frameWidth / 2 - playerWidth, frameHeight - playerHeight), cv::Size(playerWidth, playerHeight), lasers);

//Asteroid dimensions
    int asteroidWidth = 10;
    int asteroidHeight = 10;

    std::vector<Asteroid> asteroids;
    for(int i = 0; i < ASTEROID_CAPACITY; ++i) {
        asteroids.push_back(Asteroid(cv::Size(laserWidth, laserHeight), frameHeight));
    }

    Game game(player, asteroids, frameWidth, frameHeight);
    game.run();
    return 0;
}
