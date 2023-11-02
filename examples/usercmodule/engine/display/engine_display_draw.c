#include "engine_display_draw.h"
#include "engine_display_common.h"

#include "../math/vector3.h"
#include "../math/rectangle.h"

#include <string.h>


inline bool is_xy_inside_viewport(int32_t x, int32_t y, rectangle_class_obj_t *camera_viewport){
    vector2_class_obj_t* pos = MP_OBJ_TO_PTR(camera_viewport->pos);
    vector2_class_obj_t* size = MP_OBJ_TO_PTR(camera_viewport->size);
    if( x >= (int32_t)mp_obj_get_float(pos->x) &&
        y >= (int32_t)mp_obj_get_float(pos->y) &&
        x < (int32_t)mp_obj_get_float(pos->x)+mp_obj_get_float(size->x) &&
        y < (int32_t)mp_obj_get_float(pos->y)+mp_obj_get_float(size->y)){

        return true;
    }

    return false;
}


inline bool is_xy_inside_screen(int32_t x, int32_t y){
    if(x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT){
        return true;
    }

    return false;
}


void engine_draw_pixel(uint16_t color, int32_t x, int32_t y, engine_camera_node_class_obj_t *camera){
    uint16_t *screen_buffer = engine_get_active_screen_buffer();

    if(camera == NULL){
        if(is_xy_inside_screen(x, y)){
            screen_buffer[y*SCREEN_WIDTH + x] = color;
        }else{
            ENGINE_WARNING_PRINTF("Tried to draw pixel outside of screen bounds, clipped");
        }
    }else{
        vector3_class_obj_t *camera_position = camera->position;
        rectangle_class_obj_t *camera_viewport = camera->viewport;
        vector2_class_obj_t* pos = MP_OBJ_TO_PTR(camera_viewport->pos);
        vector2_class_obj_t* size = MP_OBJ_TO_PTR(camera_viewport->size);

        int32_t result_x = ((int32_t)mp_obj_get_float(pos->x)) + (x - (int32_t)mp_obj_get_float(camera_position->x));
        int32_t result_y = ((int32_t)mp_obj_get_float(pos->y)) + (y - (int32_t)mp_obj_get_float(camera_position->y));

        if(is_xy_inside_viewport(result_x, result_y, camera_viewport)){
            screen_buffer[result_y*SCREEN_WIDTH + result_x] = color;
        }else{
            ENGINE_WARNING_PRINTF("Tried to draw pixel (%0.3f, %0.3f) outside of viewport bounds, clipped", (double)result_x, (double)result_y);
        }
    }
}

void engine_fill_color(uint16_t color, int32_t x, int32_t y, int32_t sx, int32_t sy, engine_camera_node_class_obj_t *camera) {
    uint16_t *screen_buffer = engine_get_active_screen_buffer();
    if(camera == NULL){
        if(x < 0) x = 0;
        if(x + sx > SCREEN_WIDTH) sx = SCREEN_WIDTH - x;
        if(y < 0) y = 0;
        if(y + sy > SCREEN_HEIGHT) sy = SCREEN_HEIGHT - y;
        if(sx != 0 && sy != 0){
            //screen_buffer[y*SCREEN_WIDTH + x] = color;
            int32_t pos = y*SCREEN_WIDTH + x;
            const int32_t width_difference = SCREEN_WIDTH - sx;
            for(int y = 0; y < sy; y++) {
                for(int x = 0; x < sx; x++) {
                    screen_buffer[pos++] = color;
                }
                pos += width_difference;
            }
        }else{
            ENGINE_WARNING_PRINTF("Tried to draw zero-size rectangle");
        }
    }else{
        vector3_class_obj_t *camera_position = camera->position;
        rectangle_class_obj_t *camera_viewport = camera->viewport;
        vector2_class_obj_t* pos = MP_OBJ_TO_PTR(camera_viewport->pos);
        vector2_class_obj_t* size = MP_OBJ_TO_PTR(camera_viewport->size);

        int32_t result_x = ((int32_t)mp_obj_get_float(pos->x)) + (x - (int32_t)mp_obj_get_float(camera_position->x));
        int32_t result_y = ((int32_t)mp_obj_get_float(pos->y)) + (y - (int32_t)mp_obj_get_float(camera_position->y));
        if(result_x < (int32_t)mp_obj_get_float(pos->x)) x = mp_obj_get_float(pos->x);
        if(result_x + sx > (int32_t)mp_obj_get_float(size->x)) sx = (int32_t)mp_obj_get_float(size->x) - result_x;
        if(result_y < (int32_t)mp_obj_get_float(pos->y)) y = mp_obj_get_float(pos->y);
        if(result_y + sy > (int32_t)mp_obj_get_float(size->y)) sy = (int32_t)mp_obj_get_float(size->y) - result_y;

        if(sx != 0 && sy != 0){
            int32_t pos = result_y*SCREEN_WIDTH + result_x;
            const int32_t width_difference = SCREEN_WIDTH - sx;
            for(int y = 0; y < sy; y++) {
                for(int x = 0; x < sx; x++) {
                    screen_buffer[pos++] = color;
                }
                pos += width_difference;
            }
        }else{
            ENGINE_WARNING_PRINTF("Tried to draw zero-size rectangle");
        }
    }
}

void engine_draw_fill_screen_buffer(uint16_t color, uint16_t *screen_buffer){
    memset(screen_buffer, color, SCREEN_BUFFER_SIZE_BYTES);
}


void engine_draw_blit(uint16_t *pixels, int32_t x, int32_t y, uint16_t width, uint16_t height){
    // if(x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT){
    //     ENGINE_WARNING_PRINTF("Tried to blit of screen bounds");
    //     return;
    // }

    // uint16_t *screen_buffer = engine_get_active_screen_buffer();
    // uint16_t pixels_in_screen_buffer_start = (y * width) + x;

    // for(uint16_t ihx=0; ihx<height; ihx++){
    //     for(uint16_t width, )
    // }
}
