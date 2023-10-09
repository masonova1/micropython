#include "engine.hpp"

// Set/define default values for static member variables
uint8_t Engine::tick_rate = 30;
uint8_t Engine::physics_tick_rate = 15;


void Engine::start(){

}


void Engine::set_tick_rate(uint8_t new_tick_rate){
    tick_rate = new_tick_rate;
}

uint8_t Engine::get_tick_rate(){
    return tick_rate;
}


void Engine::set_physics_tick_rate(uint8_t new_physics_tick_rate){
    physics_tick_rate = new_physics_tick_rate;
}

uint8_t Engine::get_physics_tick_rate(){
    return physics_tick_rate;
}


void Engine::add_node(){
    nodes.push_back(std::unique_ptr<Node>(new Node()));
}

void Engine::remove_node(uint16_t node_index){
    // nodes.erase(node_index);
}


extern "C" {
    #include <engine_module.h>

    mp_obj_t engine_start(){
        Engine::start();
        return mp_const_none;
    }


    mp_obj_t engine_set_tick_rate(mp_obj_t new_tick_rate){
        Engine::set_tick_rate(mp_obj_get_int(new_tick_rate));
        return mp_const_none;
    }

    mp_obj_t engine_get_tick_rate(){
        return mp_obj_new_int(Engine::get_tick_rate());
    }


    mp_obj_t engine_set_physics_tick_rate(mp_obj_t new_tick_rate){
        Engine::set_physics_tick_rate(mp_obj_get_int(new_tick_rate));
        return mp_const_none;
    }

    mp_obj_t engine_get_physics_tick_rate(){
        return mp_obj_new_int(Engine::get_physics_tick_rate());
    }


    void engine_add_node(){

    }
}