//
// Created by gus3000 on 10/02/2024.
//

#ifndef RATHENA_AIPLAYER_HPP
#define RATHENA_AIPLAYER_HPP

#include <string>
#include <random>

#include "pc.hpp"

class AiPlayer {
protected:
    map_session_data *_md;
    int x;
    int y;

    std::mt19937 gen;
    std::uniform_int_distribution<> distr;

    void getMonsters();

public:
    explicit AiPlayer(map_session_data *);

    void update();

    // ---- DISPLAY ----
    void sendClientMessage(const std::string &message);

    void displayStatus();
    void displayPath();
};


#endif //RATHENA_AIPLAYER_HPP
