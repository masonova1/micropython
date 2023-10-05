#include "engine.hpp"

// Set/define default values for static member variables
uint8_t Engine::tick_rate = 30;
uint8_t Engine::physics_tick_rate = 15;


void Engine::start(){

}

extern "C" {
    #include <engine_module.h>

    mp_obj_t engine_start() {
        Engine::start();
        return mp_const_none;
    }
}