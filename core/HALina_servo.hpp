//
// Created by Kamil on 02.01.2020.
//

#ifndef PROJECT_NAME_HALINA_SERVO_HPP
#define PROJECT_NAME_HALINA_SERVO_HPP

namespace halina {

    class Servo {
    public:
        Servo();

        virtual void set(float position);

        virtual float get();
    }; // class Servo

} // namespace hal

#endif //PROJECT_NAME_HALINA_SERVO_HPP
