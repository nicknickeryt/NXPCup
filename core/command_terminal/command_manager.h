#ifndef ZUMO_COMMAND_MANAGER_H
#define ZUMO_COMMAND_MANAGER_H

#include "cyclicBuffer.hpp"
#include <functional>
#include <utility>
#include "Command.h"

class PrintManager {
public:
    char buff[10] = {0};
    virtual void print(char) = 0;

    size_t print(const char *s) {
        size_t size = 0;
        char c;
        while ((c = *s) != '\0') {
            print(c);
            s++;
            size++;
        }
        return size;
    }

    void print(uint16_t value) {
        sprintf(buff, "%d", value);
        print(buff);
    }

    void print(uint32_t value) {
        sprintf(buff, "%lu", value);
        print(buff);
    }

    void print(int16_t value) {
        sprintf(buff, "%d", value);
        print(buff);
    }

    void print(float value) {
        sprintf(buff, "%.2f", value);
        print(buff);
    }

    virtual void deinit() = 0;
};

extern volatile bool print_flag;
template <int size, char end_char, bool echo>
class CommandManager : public PrintManager {

    constexpr static size_t buff_size = 50;
    CyclicBuffer_data<char, buff_size> buffer_rx;
    CyclicBuffer_data<char, buff_size> buffer_tx;
    uint8_t commands_in_buffer = 0;

    std::function<void(void)> enable_interrupts = nullptr;
    std::function<void(void)> disable_interrupts = nullptr;

    std::array<Command, size> commands;

    std::function<void(char)> print_handler = nullptr;

    uint8_t command_title_len = 0;
public:
    explicit CommandManager(std::function<void(void)> enable_interrupts, std::function<void(void)> disable_interrupts,  std::array<Command, size> commands) :
        enable_interrupts(std::move(enable_interrupts)), disable_interrupts(std::move(disable_interrupts)), commands(commands), print_handler(nullptr) {
    }

    bool init(const std::function<void(char)>& print_handler_) {
        if (print_handler == nullptr) {
            print_handler = print_handler_;
            return true;
        }
        return false;
    }

    void deinit() override {
        print_handler = nullptr;
    }

    void print(char c) override {
        buffer_tx.append(c);
    }

    bool is_enabled() {
        return (print_handler != nullptr);
    }

    bool put_char(char c) {
        if constexpr (echo) {
            print(c);
        }

        buffer_rx.append(c);
        if (c == end_char) {
            commands_in_buffer++;
        }

        if (print_handler) {
            return true;
        }
        return false;
    }

    void run() {
        if (!is_enabled()) {
            return;
        }

        if (!buffer_tx.isEmpty()) {
            if (print_handler) {
                print_handler(buffer_tx.get());
            }
        }

        disable_interrupts();
        uint8_t commands_in_fifo_local = commands_in_buffer;
        commands_in_buffer = 0;
        enable_interrupts();

        while (commands_in_fifo_local--) {
            command_title_len = 0;
            char* cmd_buffer = copy_from_fifo_to_buffer();
            parse(cmd_buffer);
        }
    }

    void parse(char* data) {
        for (uint8_t i = 0; i < 20; i ++) {
            if (data[i] == ' ' || data[i] == '\0') {
                data[i] = '\0';
                command_title_len = i;
                break;
            }
        }
        for (auto command : commands) {
            if (command.parse(data, command_title_len)) {
                return;
            }
        }
    }

    char *copy_from_fifo_to_buffer() {
        static std::array<char, 100> cmd_buffer;
        auto it = cmd_buffer.begin();
        disable_interrupts();
        while (buffer_rx.getSize() != 0 && it != cmd_buffer.end()) {
            *it = buffer_rx.get();
            if(*it == end_char) {
                *it = '\0';
                break;
            }
            it++;
        }
        enable_interrupts();
        return cmd_buffer.data();
    }
};


#endif //ZUMO_COMMAND_MANAGER_H
