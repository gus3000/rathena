//
// Created by gus3000 on 10/02/2024.
//

#ifndef RATHENA_AIPLAYER_HPP
#define RATHENA_AIPLAYER_HPP

#include <string>
#include <random>

#include "pc.hpp"
#include "mappoint.hpp"
#include "aitargetmob.hpp"

#define AI_PLAYER_ATTACK_RANGE 10

class AiPlayer {
protected:
    map_session_data *_md;
    AiTargetMob *target;
    MapPoint *destination;

    std::mt19937 gen;
    std::uniform_int_distribution<short> distr;


//    AiTargetMob* get_closest_mob();
    void target_closest_mob();

    void remove_target_if_invalid();

    bool attack_target();

    void randomWalk();

    static std::map<int, AiPlayer *> players;

public:
    [[nodiscard]] short x() const { return _md->bl.x; }

    [[nodiscard]] short y() const { return _md->bl.y; }

    explicit AiPlayer(map_session_data *);

    void update();

    // ---- DISPLAY ----
    void sendClientMessage(const std::string &message);

    void displayStatus();

    void displayPath();

    void navigateToDestination();

    static AiPlayer *getPlayer(map_session_data *md);

    double distance(const AiTargetMob *mob) const;
};

#endif //RATHENA_AIPLAYER_HPP
