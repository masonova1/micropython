#ifndef ENGINE_DISPLAY_DRAW_H
#define ENGINE_DISPLAY_DRAW_H

#include <stdint.h>
#include "../nodes/camera_node.h"

// If 'camera' is not NULL, rotate pixel around camera and offset/clip to viewport,
// otherwise, make sure pixel is within screen bounds and put into screen buffer
void engine_draw_pixel(uint16_t color, int32_t x, int32_t y, engine_camera_node_class_obj_t* camera);

// Fill entire screen buffer with a color
void engine_draw_fill_screen_buffer(uint16_t color, uint16_t *screen_buffer);

// Fill rectangle at 'x' and 'y' given 'width' and 'height' in color 'color'
void engine_fill_color(uint16_t color, int32_t x, int32_t y, int32_t width, int32_t height, engine_camera_node_class_obj_t *camera);

// Blits pixels to screen at 'x' and 'y' given 'width' and 'height'
void engine_draw_blit(uint16_t *pixels, int32_t x, int32_t y, int32_t width, int32_t height, engine_camera_node_class_obj_t *camera);

#endif  // ENGINE_DISPLAY_DRAW_H
