//
// Created by gus3000 on 10/02/2024.
//

#include "aiPlayer.hpp"
#include "aitargetmob.hpp"
#include "astar.hpp"
#include "astar_node.hpp"

#include <cmath>
#include <sstream>
#include <random>

#include <common/nullpo.hpp>
#include <common/showmsg.hpp>
#include <common/utils.hpp>

std::map<int, AiPlayer *> AiPlayer::players;

AiPlayer::AiPlayer(map_session_data *md) : _md(md) {
    std::random_device randomDevice; // obtain a random number from hardware
    gen = std::mt19937(randomDevice()); // seed the generator
    distr = std::uniform_int_distribution<short>(-3, 3); // define the range
    this->target = nullptr;
    this->destination = nullptr;
    this->update();
    navigateToDestination();
}

void AiPlayer::randomWalk() {
    short destX = x() + distr(gen);
    short destY = y() + distr(gen);

    unit_walktoxy(&(_md->bl), destX, destY, 8);
}

void AiPlayer::update() {
    displayStatus();

    //    void clif_navigateTo(map_session_data *sd, const char* mapname, uint16 x, uint16 y, uint8 flag, bool hideWindow, uint16 mob_id );
//    clif_navigateTo(_md, "moc_pryd01", 90, 105, NAV_NONE, true, 0);

//    remove_target_if_invalid();
//    if (this->target == nullptr) {
//        try {
//            target_closest_mob();
//        } catch (std::range_error &) {}
//    }
//    attack_target();
}

void AiPlayer::sendClientMessage(const std::string &message) {
    const std::string paddedMessage = message + "\n";
    clif_displaymessage(this->_md->fd, paddedMessage.data());
    ShowInfo(paddedMessage.data());
}

void AiPlayer::displayStatus() {
    this->sendClientMessage(string_format("Position %d,%d", x(), y()));

    map_data* m = map_getmapdata(_md->mapindex);
//    this->sendClientMessage(string_format("Map dim (cells) : %d,%d", m->xs, m->ys));
//    this->sendClientMessage(string_format("Map dim (blocks) : %d,%d", m->bxs, m->bys));
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

void AiPlayer::target_closest_mob() {
    delete this->target;

    int16 map_id = this->_md->bl.m;

    std::vector<mob_data *> mobs;
    map_foreachinmap(aiplayer_monster_add_to_candidates, map_id, BL_MOB, &mobs);

    if (mobs.empty())
        throw std::range_error("no mob found");

    mob_data *closest_mob = nullptr;
    double closest_distance = DBL_MAX;
    for (auto md: mobs) {
        short mobX = md->bl.x;
        short mobY = md->bl.y;

        int distX = std::abs(mobX - this->x());
        int distY = std::abs(mobY - this->y());

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

        if (closest_mob == nullptr)
            throw std::logic_error("mobs found but no mob is targetable");

        this->target = new AiTargetMob(closest_mob);
    }
}

void AiPlayer::remove_target_if_invalid() {
    if (target == nullptr)
        return;

    if (status_isdead(target->block())) {
        delete this->target;
        this->target = nullptr;
    }
//    if (!unit_can_attack(&(this->_md->bl), target->mob_id())) {
//    }
}

bool AiPlayer::attack_target() {
    if (target == nullptr)
        return false;
    //throw std::logic_error("trying to attack null target");
    this->sendClientMessage(string_format("Attacking mob : [%d] %s (%d,%d) (distance : %f), can_attack = %d",
                                          target->mob_id(),
                                          target->name(),
                                          target->x(),
                                          target->y(),
                                          this->distance(target),
                                          unit_can_attack(&(this->_md->bl), target->mob_id())
    ));
//
    // check if mob is valid target (this does not care about range)
    if (!unit_can_attack(&(this->_md->bl), target->mob_id())) {
        this->sendClientMessage("Cannot attack");
        return false;
    }

    if (this->distance(target) > AI_PLAYER_ATTACK_RANGE) {
        bool walkSuccess =
                unit_walktobl(&(this->_md->bl), target->block(), AI_PLAYER_ATTACK_RANGE, 2);
        this->sendClientMessage(string_format("walk successful ? %d", walkSuccess));
    } else {
        auto attack_successful =
                unit_attack(&(this->_md->bl), target->mob_id(), true);
        this->sendClientMessage(string_format("attack successful ? %d", attack_successful));
    }
    return true;
}

void AiPlayer::navigateToDestination() {
//    clif_navigateTo(_md, "moc_pryd01", 90, 105, NAV_NONE, true, 0);
//    auto startMapData = map_getmapdata(_md->mapindex);
    auto start = astar::State(_md->mapindex,_md->bl.x, _md->bl.y);
    this->sendClientMessage(start.to_string().c_str());
}


AiPlayer *AiPlayer::getPlayer(map_session_data *md) {
    int id = md->fd;
    try {
        return players.at(id);
    } catch (std::out_of_range &) {
        auto player = new AiPlayer(md);
        players[id] = player;
        return player;
    }
}

double AiPlayer::distance(const AiTargetMob *mob) const {
    int distX = abs(this->x() - mob->x());
    int distY = abs(this->y() - mob->y());

    return std::sqrt(distX * distX + distY * distY);
}




