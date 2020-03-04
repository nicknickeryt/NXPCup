#include "Command.h"

namespace parser {

    template<> double parse<double>(std::string_view const s) {
        return atof(s.data());
    }

    template<> int parse<int>(std::string_view const s) {
        return atoi(s.data());
    }

    template<> float parse<float>(std::string_view const s) {
        return atof(s.data());
    }
}