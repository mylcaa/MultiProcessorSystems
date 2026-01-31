#include <vector>

#include "game.hpp"
#include "player.hpp"
#include "laser.hpp"
#include "asteroid.hpp"  

int main() {
//Setup
    std::vector<Laser> lasers;
    for(int i=0; i < LASER_CAPACITY; ++i) {
        lasers.push_back(Laser(cv::Size(LASER_WIDTH, LASER_HEIGHT)));
    }

    Player player(cv::Point(FRAME_WIDTH / 2 - PLAYER_WIDTH, FRAME_HEIGHT - PLAYER_HEIGHT), cv::Size(PLAYER_WIDTH, PLAYER_HEIGHT), lasers);

    std::vector<Asteroid> asteroids;
    for(int i = 0; i < ASTEROID_CAPACITY; ++i) {
        asteroids.push_back(Asteroid(cv::Size(ASTEROID_WIDTH, ASTEROID_HEIGHT), FRAME_HEIGHT));
    }
//Game start
    Game game(player, asteroids, FRAME_WIDTH, FRAME_HEIGHT, 2);
    game.run();

    return 0;
}
