#pragma once

class Kitty {
  private:
    int motor;
    Kitty() = default;

  public:
    void init();
    Kitty(int motor);

    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }
};