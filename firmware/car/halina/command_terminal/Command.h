#ifndef ZUMO_COMMAND_H
#define ZUMO_COMMAND_H

#include <functional>
#include <utility>
#include <tuple>
#include <array>
#include <cstring>
#include <string_view>
#include <cstdint>

class Command {
    const char* name;
    size_t  name_len = 0;
    const char* description;
public:
    std::function<void(const char*)> callback_handler;

public:
    Command(const char* name, const char* description ,std::function<void(const char*)> callback_handler ) : name(name), name_len(strlen(name)), description(description), callback_handler(std::move(callback_handler)) {

    }

    bool parse(char * data, size_t data_len) {
        if (data_len != name_len) {
            return false;
        }

        if (memcmp(data, name, name_len) == 0) {
            callback_handler((char *)data + name_len + 1);
            return true;
        }
        return false;
    }
};

namespace parser {
    template<class T> T parse(std::string_view const s);

    template<class T0, class... T>  std::tuple<T0, T...>  get(std::string_view const s) {
        const auto spacePos = s.find_first_of(' ', 0);

        auto val =  parse<T0>(s);

        if constexpr (sizeof ...(T)) {
            return std::tuple_cat(std::make_tuple(val), get<T...>(s.substr(spacePos+ 1)));
        } else {
            return std::make_tuple(val);
        }
    }

    template<typename T, int size>
    std::array<T, size> get (const char * data) {
        std::array<T, size> return_data {};

        if constexpr (std::is_same<T, float>() ) {
            char *end;

            for (int i=0; i < size; i++) {
                return_data[i] = strtod(data, &end);
                data = end;
            }
        } else if constexpr (std::is_same<int, T>() || std::is_same<uint32_t, T>()) {
            char *end;

            for (int i=0; i < size; i++) {
                return_data[i] = strtol(data, &end, 10);
                data = end;
            }
        }
        return return_data;
    }
}





#endif //ZUMO_COMMAND_H
