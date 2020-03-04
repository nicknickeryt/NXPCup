#include "gtest/gtest.h"
#include <vector>
#include <gmock/gmock-spec-builders.h>
#include "command_terminal/Command.h"
#include <gmock/gmock.h>
#include "command_terminal/command_manager.h"

using ::testing::StrictMock;
using ::testing::_;

class CommandBaseMock : public Command {
public:
    MOCK_METHOD(void, callback, (const char *));

    explicit CommandBaseMock(const char* name) : Command(name, ([this](const char* ch) {this->callback(ch);})) { }
};

TEST(COMMAND, create) {
    CommandBaseMock start("start");
    const char* data = "start 12345";
    EXPECT_CALL(start, callback(data + 6));
    EXPECT_EQ(start.parse((char*)data, 5), true);
//
    EXPECT_CALL(start, callback(_)).Times(0);
    EXPECT_EQ(start.parse("data", 5), false);
}

TEST(COMMAND, value_float) {
    const char *data0 = "12 5.763 344";
    auto[f, g, h] = parser::get<int, float, int>(data0);
    EXPECT_EQ(f, 12);
    EXPECT_EQ(g, (float)5.763);
    EXPECT_EQ(h, 344);

    const char* data1 = "2.345 4.235";
    auto [x1, y1] = parser::get<float, float>(data1);
    EXPECT_EQ(x1, (float)2.345);
    EXPECT_EQ(y1, (float)4.235);

    const char* data2 = "-234 5432";
    auto [x2, y2] = parser::get<int, int>(data2);
    EXPECT_EQ(x2, -234);
    EXPECT_EQ(y2, 5432);

    const char* data3 = "2345";
    auto [x3] = parser::get<int>(data3);
    EXPECT_EQ(x3, 2345);
}

void enable_interrupts() {}

void disable_interrupts() {}

void print_function(char c) {
    (void)c;
}

TEST (COMMAND_MANAGER, basic) {
    CommandBaseMock jeden("jeden");
    CommandBaseMock dwa("dwa");

    CommandManager<2, '\n', true> command_manager(&enable_interrupts, &disable_interrupts, {jeden, dwa});
    command_manager.init(print_function);

    const char * data = "jeden sdfg\ndwa 2345\n";

    for (size_t i =0; i < strlen(data); i++) {
        command_manager.put_char(data[i]);
    }

    EXPECT_CALL(jeden, callback(_));
    EXPECT_CALL(dwa, callback(_));

    command_manager.run();
}

char data_table[3][100];

void callback1(const char* data) {
    static int data_len = 0;
    strcpy(data_table[data_len], data);
    data_len++;
}

TEST (COMMAND_MANAGER, multi_commands) {
    CommandManager<2, '\n', true> command_manager(&enable_interrupts, &disable_interrupts, {
                Command("jeden", callback1),
                Command("dwa", callback1)
             });
    command_manager.init(print_function);

    const char * data = "jeden sdfg\ndwa 2345\n";

    for (size_t i =0; i < strlen(data); i++) {
        command_manager.put_char(data[i]);
    }

    command_manager.run();

    EXPECT_EQ(std::string("sdfg"), std::string(data_table[0]));
    EXPECT_EQ(std::string("2345"), std::string(data_table[1]));
}

float ff1, ff2;

void two_floats(const char* data) {
    auto [f1, f2] = parser::get<float, float>(data);
    ff1 = f1;
    ff2 = f2;
}

TEST(COMMAND_MANAGER, two_floats) {
    CommandManager<1, '\n', true> command_manager(&enable_interrupts, &disable_interrupts, {
            Command("floats", two_floats)
    });
    command_manager.init(print_function);

    const char * data = "floats 2.456 3.654\n";

    for (size_t i =0; i < strlen(data); i++) {
        command_manager.put_char(data[i]);
    }

    command_manager.run();

    EXPECT_EQ(ff1, (float)(2.456));
    EXPECT_EQ(ff2, (float)(3.654));
}

int functionNUmber = 0;
void question1(const char * data) {
    functionNUmber = 1;
}

void question2(const char * data) {
    functionNUmber = 2;
}

void question3(const char * data) {
    functionNUmber = 3;
}

void question4(const char * data) {
    functionNUmber = 4;
}

TEST(COMMAND_MANAGER, question) {
    CommandManager<4, '\n', true> command_manager(&enable_interrupts, &disable_interrupts, {
            Command("t?", question1),
            Command("n?", question2),
            Command("t", question3),
            Command("n", question4)
    });
    command_manager.init(print_function);


    const char * data1 = "t 1\n";
    for (size_t i =0; i < strlen(data1); i++) {
        command_manager.put_char(data1[i]);
    }
    command_manager.run();
    EXPECT_EQ(functionNUmber, 3);

    const char * data2 = "t?\n";
    for (size_t i =0; i < strlen(data2); i++) {
        command_manager.put_char(data2[i]);
    }
    command_manager.run();
    EXPECT_EQ(functionNUmber, 1);
}

