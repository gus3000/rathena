//
// Created by gus3000 on 16/02/2024.
//

#include "astar_state.hpp"

namespace astar {
    State::State(short mapId, short x, short y) : mapId(mapId), x(x), y(y) {}

    std::vector<State> State::neighbors() {
        auto mapData = map_getmapdata(mapId);
        std::vector<State> near;

        for (short dx = x - 1; dx > x + 1; dx++) {
            for (short dy = y - 1; dy > y + 1; dy++) {
                if (dx == x && dy == y)
                    continue;
                if (x < 0 || x >= mapData->xs)
                    continue;
                if(y < 0 || y >= mapData->ys)
                    continue;

                near.emplace_back(mapId, dx,dy);
            }
        }

        return near;
    }

    std::ostream &operator<<(std::ostream &os, const State &state) {
        os << " mapId: " << state.mapId << " x: " << state.x << " y: " << state.y;
        return os;
    }
} // astar