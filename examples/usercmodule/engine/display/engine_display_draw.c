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

void engine_fill_color(uint16_t color, int32_t x, int32_t y, int32_t width, int32_t height, engine_camera_node_class_obj_t *camera) {
    uint16_t *screen_buffer = engine_get_active_screen_buffer();
    if(camera == NULL){
        if(x < 0) {
            width += x;
            x = 0;
        }
        if(y < 0)  {
            height += y;
            y = 0;
        }
        if(x + width > SCREEN_WIDTH) width = SCREEN_WIDTH - x;
        if(y + height > SCREEN_HEIGHT) height = SCREEN_HEIGHT - y;
        if(width > 0 && height > 0){
            //screen_buffer[y*SCREEN_WIDTH + x] = color;
            int32_t pos = y*SCREEN_WIDTH + x;
            const int32_t width_difference = SCREEN_WIDTH - width;
            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) {
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

        ENGINE_WARNING_PRINTF("Viewport rectangle at (%0.3f, %0.3f) with size (%0.3f, %0.3f)", (double)mp_obj_get_float(pos->x), (double)mp_obj_get_float(pos->y), (double)mp_obj_get_float(size->x), (double)mp_obj_get_float(size->y));

        int32_t result_x = ((int32_t)mp_obj_get_float(pos->x)) + (x - (int32_t)mp_obj_get_float(camera_position->x));
        int32_t result_y = ((int32_t)mp_obj_get_float(pos->y)) + (y - (int32_t)mp_obj_get_float(camera_position->y));
        if(result_x < (int32_t)mp_obj_get_float(pos->x)) {
            width -= (int32_t)mp_obj_get_float(pos->x) - result_x;
            result_x = (int32_t)mp_obj_get_float(pos->x);
        }
        if(result_y < (int32_t)mp_obj_get_float(pos->y)) {
            height -= (int32_t)mp_obj_get_float(pos->y) - result_y;
            result_y = (int32_t)mp_obj_get_float(pos->y);
        }
        if(result_x + width > (int32_t)mp_obj_get_float(size->x)) width = (int32_t)mp_obj_get_float(size->x) - result_x;
        if(result_y + height > (int32_t)mp_obj_get_float(size->y)) height = (int32_t)mp_obj_get_float(size->y) - result_y;

        ENGINE_WARNING_PRINTF("Drawing rectangle at (%0.3f, %0.3f) with size (%0.3f, %0.3f)", (double)result_x, (double)result_y, (double)width, (double)height);


        if(width > 0 && height > 0){
            int32_t pos = result_y*SCREEN_WIDTH + result_x;
            const int32_t width_difference = SCREEN_WIDTH - width;
            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) {
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


void engine_draw_blit(uint16_t *pixels, int32_t x, int32_t y, int32_t width, int32_t height, engine_camera_node_class_obj_t *camera){
    uint16_t *screen_buffer = engine_get_active_screen_buffer();
    const int32_t old_width = width;
    int32_t p_add = 0;
    int32_t p_offs = 0;
    if(camera == NULL){
        if(y < 0) {
            p_offs = -y * width;
            height += y;
            y = 0;
        }
        if(x < 0) {
            p_add = -x;
            width += x;
            x = 0;
        }
        if(x + width > SCREEN_WIDTH) width = SCREEN_WIDTH - x;
        if(y + height > SCREEN_HEIGHT) height = SCREEN_HEIGHT - y;
        if(width != 0 && height != 0){
            //screen_buffer[y*SCREEN_WIDTH + x] = color;
            int32_t pos = y*SCREEN_WIDTH + x;
            int32_t p_pos = 0;
            const int32_t width_difference = SCREEN_WIDTH - width;
            const int32_t p_width_difference = old_width - width;
            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) {
                    screen_buffer[pos++] = pixels[p_offs + p_pos++];
                }
                pos += width_difference;
                p_pos += p_width_difference + p_add;
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
        if(result_y < (int32_t)mp_obj_get_float(pos->y)) {
            p_offs = ((int32_t)mp_obj_get_float(pos->y) - result_y) * width;
            height -= (int32_t)mp_obj_get_float(pos->x) - result_x;
            result_y = mp_obj_get_float(pos->y);
        }
        if(result_x < (int32_t)mp_obj_get_float(pos->x)) {
            p_add = (int32_t)mp_obj_get_float(pos->x) - result_x;
            width -= (int32_t)mp_obj_get_float(pos->x) - result_x;
            result_x = mp_obj_get_float(pos->x);
        }
        if(result_x + width > (int32_t)mp_obj_get_float(size->x)) width = (int32_t)mp_obj_get_float(size->x) - result_x;
        if(result_y + height > (int32_t)mp_obj_get_float(size->y)) height = (int32_t)mp_obj_get_float(size->y) - result_y;

        if(width != 0 && height != 0){
            int32_t pos = result_y*SCREEN_WIDTH + result_x;
            int32_t p_pos = 0;
            const int32_t width_difference = SCREEN_WIDTH - width;
            const int32_t p_width_difference = old_width - width;
            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) {
                    screen_buffer[pos++] = pixels[p_offs + p_pos++];
                }
                pos += width_difference;
                p_pos += p_width_difference + p_add;
            }
        }else{
            ENGINE_WARNING_PRINTF("Tried to draw zero-size rectangle");
        }
    }
}
