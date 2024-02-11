//
// Created by gus3000 on 10/02/2024.
//

#include "aiPlayer.hpp"

#include <climits>
#include <cmath>
#include <sstream>
#include <random>

#include <common/nullpo.hpp>
#include <common/showmsg.hpp>
#include <common/utils.hpp>

AiPlayer::AiPlayer(map_session_data *md) : _md(md) {
    std::random_device randomDevice; // obtain a random number from hardware
    gen = std::mt19937(randomDevice()); // seed the generator
    distr = std::uniform_int_distribution<>(-3, 3); // define the range

    this->update();
}

int16 AiPlayer::getX() const {
    return x;
}

int16 AiPlayer::getY() const {
    return y;
}

void AiPlayer::randomWalk() {
    int16 destX = x + distr(gen);
    int16 destY = y + distr(gen);

    unit_walktoxy(&(_md->bl), destX, destY, 8);
}

void AiPlayer::update() {
    this->x = _md->bl.x;
    this->y = _md->bl.y;
    attack_closest_mob();
}

void AiPlayer::sendClientMessage(const std::string &message) {
    const std::string paddedMessage = message + "\n";
    clif_displaymessage(this->_md->fd, paddedMessage.data());
    ShowInfo(paddedMessage.data());
}

void AiPlayer::displayStatus() {
    this->sendClientMessage(string_format("Position %d,%d", this->x, this->y));
}

void AiPlayer::displayPath() {
    walkpath_data walkpath = _md->ud.walkpath;
    std::stringstream tileStream;
    tileStream << "Path :";
    for (int i = 0; i < walkpath.path_len; i++) {
        auto direction = walkpath.path[i];
        switch (direction) {
            case DIR_NORTH:
                tileStream << "NORTH";
                break;
            case DIR_NORTHWEST:
                tileStream << "NORTHWEST";
                break;
            case DIR_WEST:
                tileStream << "WEST";
                break;
            case DIR_SOUTHWEST:
                tileStream << "SOUTHWEST";
                break;
            case DIR_SOUTH:
                tileStream << "SOUTH";
                break;
            case DIR_SOUTHEAST:
                tileStream << "SOUTHEAST";
                break;
            case DIR_EAST:
                tileStream << "EAST";
                break;
            case DIR_NORTHEAST:
                tileStream << "NORTHEAST";
                break;
            default:
                tileStream << "QUE";
        }
        tileStream << " ";
    }

    sendClientMessage(tileStream.str());
}

static int aiplayer_monster_add_to_candidates(struct block_list *bl, va_list ap) {
    auto *md = (struct mob_data *) bl;
    nullpo_ret(md);
//    auto player = va_arg(ap, AiPlayer*);
    auto mobs = va_arg(ap, std::vector<mob_data *>*);


    mobs->push_back(md);
    return 0;
}

mob_data *AiPlayer::closest_mob() {
    return nullptr;
}

bool AiPlayer::attack_closest_mob() {
    int16 map_id = this->_md->bl.m;

    std::vector<mob_data *> mobs;
    map_foreachinmap(aiplayer_monster_add_to_candidates, map_id, BL_MOB, &mobs);

    mob_data *closest_mob;
    double closest_distance = DBL_MAX;
    for (auto md: mobs) {
        short mobX = md->bl.x;
        short mobY = md->bl.y;

        int distX = std::abs(mobX - this->x);
        int distY = std::abs(mobY - this->y);

        double distance = std::sqrt(distX * distX + distY * distY);
        if (distance < closest_distance) {
            closest_distance = distance;
            closest_mob = md;
        }
//        ShowInfo(string_format("Analysing mob : [%d] %s (%d,%d) (distance : %f), can_attack = %d\n",
//                               md->bl.id,
//                               md->name,
//                               md->bl.x,
//                               md->bl.y,
//                               distance
//        ).c_str());
    }

    this->sendClientMessage(string_format("Attacking mob : [%d] %s (%d,%d) (distance : %f), can_attack = %d\n",
                                          closest_mob->mob_id,
                                          closest_mob->name,
                                          closest_mob->bl.x,
                                          closest_mob->bl.y,
                                          closest_distance,
                                          unit_can_attack(&(this->_md->bl), closest_mob->bl.id)
    ));

    // check if mob is valid target (this does not care about range)
    if (!unit_can_attack(&(this->_md->bl), closest_mob->bl.id)) {
        return false;
    }

    if (closest_distance > AI_PLAYER_ATTACK_RANGE) {
        bool walkSuccess = unit_walktobl(&(this->_md->bl), &(closest_mob->bl), AI_PLAYER_ATTACK_RANGE, 2);
//    this->sendClientMessage(string_format("walk successful ? %d", walkSuccess));
    } else {
        auto attack_successful = unit_attack(&(this->_md->bl), closest_mob->bl.id, true);
//    this->sendClientMessage(string_format("attack successful ? %d", attack_successful));
    }
    return true;
}




