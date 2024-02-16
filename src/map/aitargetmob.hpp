//
// Created by gus3000 on 16/02/2024.
//

#ifndef RATHENA_AITARGETMOB_HPP
#define RATHENA_AITARGETMOB_HPP

#include <map/mob.hpp>

struct AiTargetMob {
protected:
    mob_data *mob;

public:
    explicit AiTargetMob(mob_data *mob);


    [[nodiscard]] int mob_id() const { return mob->bl.id; }

    [[nodiscard]] char *name() const { return mob->name; }

    block_list *block() { return &(mob->bl); }

    [[nodiscard]] short x() const { return mob->bl.x; }

    [[nodiscard]] short y() const { return mob->bl.y; }

};

#endif //RATHENA_AITARGETMOB_HPP
