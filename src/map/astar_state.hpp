//
// Created by gus3000 on 16/02/2024.
//

#ifndef RATHENA_ASTAR_STATE_HPP
#define RATHENA_ASTAR_STATE_HPP

#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include "map.hpp"

namespace astar {

    class State {
    protected:
        short mapId;
        short x;
        short y;
    public:
        State(short mapId, short x, short y);

        std::vector<State> neighbors();

        [[nodiscard]] std::string to_string() const {
            std::stringstream ss;
            ss << *this;
            return ss.str();
        }
        friend std::ostream &operator<<(std::ostream &os, const State &state);
    };

} // astar

#endif //RATHENA_ASTAR_STATE_HPP
