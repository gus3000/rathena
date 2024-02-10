//
// Created by gus3000 on 10/02/2024.
//

#include "aiPlayer.hpp"

#include <sstream>
#include <random>

#include <common/utils.hpp>
#include <common/showmsg.hpp>

AiPlayer::AiPlayer(map_session_data *md) : _md(md) {
    std::random_device rd; // obtain a random number from hardware
    gen = std::mt19937(rd()); // seed the generator
    distr = std::uniform_int_distribution<>(-3, 3); // define the range

    this->update();
}

void AiPlayer::update() {
    this->x = _md->bl.x;
    this->y = _md->bl.y;

    int destX = x + distr(gen);
    int destY = y + distr(gen);

    unit_walktoxy(&(_md->bl), destX, destY, 8);
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

void AiPlayer::getMonsters() {


}



