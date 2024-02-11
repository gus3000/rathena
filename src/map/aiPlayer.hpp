//
// Created by gus3000 on 10/02/2024.
//

#ifndef RATHENA_AIPLAYER_HPP
#define RATHENA_AIPLAYER_HPP

#include <string>
#include <random>

#include "pc.hpp"

#define AI_PLAYER_ATTACK_RANGE 10

class AiPlayer {
protected:
    map_session_data *_md;
    int16 x;
    int16 y;

    std::mt19937 gen;
    std::uniform_int_distribution<> distr;

    mob_data* closest_mob();
    bool attack_closest_mob();
    void randomWalk();

public:
    int16 getX() const;

    int16 getY() const;

    explicit AiPlayer(map_session_data *);

    void update();

    // ---- DISPLAY ----
    void sendClientMessage(const std::string &message);

    void displayStatus();
    void displayPath();
};


#endif //RATHENA_AIPLAYER_HPP
