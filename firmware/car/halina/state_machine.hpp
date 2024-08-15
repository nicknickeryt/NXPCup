/**
 MIT License

 Copyright (c) 2019 Michał Adamczyk

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 Modifications by Arkadiusz Bałys and Jan Macheta. Same license applies
 */
#pragma once

#include <type_traits>
#include <tuple>
#include <variant>

namespace fsm {
    template<class T> concept Event = std::is_class_v<T>;
    template<class T> concept State = std::is_class_v<T>;
    template<class T, class ...Ts> concept IsOneOf = std::disjunction_v<std::is_same<T, Ts>...>;

/************ FSM ************/
    template<State ...States> struct StateMachine {
    public:
        StateMachine() = default;
        StateMachine(States&&... states_) : states(std::forward<States>(states_)...) {}

        template<State S> requires IsOneOf<S, States...> constexpr bool in_state() const noexcept {return std::holds_alternative<S*>(current);}

        template <Event E> void handle(E const& e) { std::visit([&](auto&& statePtr) {statePtr->handle(e).exec(*this, *statePtr, e);}, current);}

        template<State S> requires IsOneOf<S, States...> S& switch_to() noexcept {
            S& s = std::get<S>(states);
            current = &s;
            return s;
        }

    private:
        std::tuple<States...> states;
        std::variant<States*...> current{ &std::get<0>(states) };
    };

/************ ACTIONS **********/

/// Defines non-transitional action
    struct NoTransition {
        template<class Machine, State S, Event E> static void exec(Machine&, S&, E const&) noexcept {}
    };

    template <class S, class E> concept HasExitRoutineFor = State<S> && Event<E> && requires (S s, E& e)  {{s.on_exit(e)};};
    template <class S, class E> concept HasEntryRoutineFor = State<S> &&  Event<E> && requires (S s, E& e)  {{s.on_entry(e)};};

/// Defines transitional action
    template<State To> struct Transition {
        template <class Machine, State S, Event E> static void exec(Machine& machine, S& prevState, E const& e) {
            if constexpr(HasExitRoutineFor<S,E>) prevState.on_exit(e);
            To& newState = machine.template switch_to<To>();
            if constexpr(HasEntryRoutineFor<To,E>) newState.on_entry(e);
        }
    };

/************ TRANSITION HELPERS ************/
    template <Event E, State To> class On {
    protected:
        static Transition<To> handle(E const&) noexcept {return {};}
    private:
        template<typename ...Ts> friend struct Transitions;
        constexpr On() = default; // Allow construction only from class Transitions
    };

/// Defines structure that merges all default-handled actions.
    template<typename ...Ts> class  Transitions : Ts... {
    public:
        using Ts::handle...;
        template<Event E> NoTransition handle(E const&) const noexcept {return {};}
    };

    template<typename ... Actions> class OneOf {
    public:
        template<typename T> OneOf(T&& arg) : options(std::forward<T>(arg)) { }
        template<typename Machine, typename State, typename Event> void exec(Machine& machine, State& state, const Event& event) { std::visit([&machine, &state, &event](auto& action) { action.exec(machine, state, event); }, options); }

    private:
        std::variant<Actions...> options;

    };

    template<typename Action> struct Maybe: OneOf<Action, NoTransition> {
        using OneOf<Action, NoTransition>::OneOf;
    };

}

