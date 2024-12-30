#include <Arduino.h>

// #define USE_EX

// #ifdef USE_EX
#include "ex_gif_animated.hpp"
// #include "ex_touch.hpp"
// #include "ex_webserver.hpp"
#else

#include <lvgl.h>
#include <CustomLGFX.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#define SD_CS_PIN 5 // Change this according to your wiring

#include "image_array/frame_00_delay_0.h"
#include "image_array/frame_02_delay_0.h"
#include "image_array/frame_03_delay_0.h"

// LGFX tft;
CustomLGFX tft;

lv_obj_t *img_obj;
// Current frame index
uint8_t current_frame = 0;

void my_lvgl_tick_handler(void)
{
  lv_tick_inc(5); // Increase LVGL tick by 5ms every time this function is called.
}

void lvgl_setup()
{
  lv_init(); // Initialize LVGL.

  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[320 * 10];
  lv_disp_draw_buf_init(&draw_buf, buf, nullptr, 320 * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = tft.width();
  disp_drv.ver_res = tft.height();
  disp_drv.flush_cb = [](lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
  {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors(reinterpret_cast<const uint16_t *>(&color_p->full), (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_disp_flush_ready(drv);
  };
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
}

// Function to create the image object and display the first frame
void create_animation()
{
  img_obj = lv_img_create(lv_scr_act());        // Create an image object on the active screen
  lv_img_set_src(img_obj, &frame_00_delay_0);   // Set the first frame
  lv_obj_align(img_obj, LV_ALIGN_CENTER, 0, 0); // Align to center
}

// Timer callback to switch between GIF frames
void switch_frames(lv_timer_t *timer)
{
  Serial.printf("current_frame = %d\n", current_frame);

  // Switch frames based on the current frame index
  switch (current_frame)
  {
  case 0:
    lv_img_set_src(img_obj, &frame_00_delay_0);
    break;
  case 2:
    lv_img_set_src(img_obj, &frame_03_delay_0);
    break;
  }

  // Force the image object to redraw
  lv_obj_invalidate(img_obj);

  current_frame++;
  if (current_frame > 3)
  {
    current_frame = 0; // Loop back to the first frame
  }
}

void setup()
{
  Serial.begin(115200);
  tft.begin();               // Initialize LovyanGFX display.
  tft.setRotation(0);        // Set display rotation if needed.
  tft.fillScreen(TFT_WHITE); // Clear the screen

  // Initialize LVGL and set up display buffer
  lvgl_setup();

  // Create the initial image object and display the first frame
  create_animation();

  // // Create a timer to switch between GIF frames every 200 ms
  lv_timer_create(switch_frames, 30, NULL);

  // Start an LVGL tick task
  const int tick_period_ms = 5;
  TimerHandle_t timer = xTimerCreate("lvgl_tick", pdMS_TO_TICKS(tick_period_ms), pdTRUE, NULL, [](TimerHandle_t xTimer)
                                     { my_lvgl_tick_handler(); });
  xTimerStart(timer, 0);
}

void loop()
{
  lv_task_handler();
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
  {
    static int rotation = (rotation + 1) % 4; // Rotation can be 0, 1, 2, or 3
    tft.setRotation(rotation);
    // Optional: clear the screen after each rotation
    tft.fillScreen(TFT_BLACK);

    // Add a small delay to avoid multiple detections from a single touch
    delay(500);
  }

  delay(5);
}

#endif
