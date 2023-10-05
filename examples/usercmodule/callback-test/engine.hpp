#ifndef ENGINE_HPP
#define ENGINE_HPP

// MicroPython header files: https://micropython-usermod.readthedocs.io/en/latest/usermods_05.html#header-files
#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

#include <cstdint>
#include <vector>
#include <stdexcept>

// Engine singleton that is used for handling main
// game parameters (starting the main game loop,
// fps/tick rate, and physics update rate)
class Engine{
    public:
        // Called in MicroPython to start the engine game loop.
        // This should be called after node callbacks have been
        // defined
        static void start();

        // Set and get 'tick_rate' which the engine uses
        // to invoke all nodes' '.tick()' callbacks
        static void set_tick_rate(uint8_t new_tick_rate);
        static uint8_t get_tick_rate(); 

        // Set and get 'physics_tick_rate' which the engine uses
        // to invoke all nodes' '.physics_tick()' callbacks
        static void set_physics_tick_rate(uint8_t physics_tick_rate);
        static uint8_t get_physics_tick_rate();
    private:
        // Rates at which node callback functions are invoked by
        // the engine. Default values are set in 'engine.cpp'
        static uint8_t tick_rate;
        static uint8_t physics_tick_rate;

        // The list of all node instances. This is used 
        // to find callbacks to call every game loop
        static std::vector<mp_obj_t> nodes;
};


#endif  // ENGINE_HPP