//
// Created by gus3000 on 10/02/2024.
//

#include "aiPlayer.hpp"

#include <common/utils.hpp>

AiPlayer::AiPlayer(map_session_data *md) : _md(md) {
    this->update();
}

void AiPlayer::update() {
    this->x = _md->bl.x;
    this->y = _md->bl.y;

    unit_walktoxy(&(_md->bl),x+1,y, 8);
}

void AiPlayer::sendClientMessage(const std::string& message) {
    clif_displaymessage(this->_md->fd, message.data());
}

void AiPlayer::displayStatus() {
    this->sendClientMessage(string_format("Position %d,%d", this->x, this->y));
}

void AiPlayer::getMonsters() {


}



