
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE /* needed for usleep()-闂団偓鐟曚�?�濞�?悽鈺眘leep閸戣姤鏆�? */
#endif

#include <stdlib.h>
#include <stdio.h>

#ifdef _MSC_VER
  #include <Windows.h> // windows缁�鑽ょ埠娑擄拷閻�?���?箒閻ㄥ�??銇旈�?鍥︽�㈤敍灞惧絹娓氭硞leep閸戣姤鏆�?
#else
  #include <unistd.h> // 閺嶅洤鍣�鎼达拷 閹绘劒绶�? usleep 閸戣姤鏆�?
  #include <pthread.h> // POSIX 缁捐法鈻兼惔锟�
#endif

#include "lvgl/lvgl.h"                           // LVGL 閻ㄥ�?鐗宠箛鍐ㄧ氨
#include "lvgl/examples/lv_examples.h"           // LVGL 缁€杞扮伐缁�?���?�?
#include "lvgl/demos/lv_demos.h"                 // LVGL 濠曟梻銇氱粙瀣��?�?
#include <SDL.h>                                 // 閻�銊ょ艾閸ユ儳鑸伴�?宀�?翻閸忋儴锟芥儳锟藉洨娈戞禒璺ㄦ埂
#include "../UI/my_ui.h"


static lv_display_t * hal_init(int32_t w, int32_t h);
// static娣囷拷妤楁�?�娈戦崙鑺ユ殶閸︺劌鍙炬禒鏍у毐閺侀�佺瑝閼冲€熺�?�閻�锟�?
// 婢圭増妲戠涵锟芥禒鑸靛▕鐠炩�?崇湴 閻ㄥ�?鍨垫慨瀣�瀵查崙鑺ユ�?�閿涘牏鐛ラ崣锝冣偓浣界翻閸忋儴锟芥儳锟藉洢鈧�鎰�袝閹界�跨潌閵嗕線绱堕弽鍥モ偓鎴�绱�?

extern void freertos_main(void);
// 婵″倹鐏夋担璺ㄦ暏娴滃挾reeRtos 娴ｈ法鏁ゆ潻娆庨�?

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init(480, 320);

  my_ui_init();

  while(1) {
    lv_timer_handler();
  }
  return 0;
}

static lv_display_t * hal_init(int32_t w, int32_t h)
{

  // 閸掓稑缂撴稉鈧�娑擄拷�?�涙�匡拷銈堢翻閸忋儳绮�? 閻�銊ょ艾�?�跺�?�?婇柨锟介惄锟�? / 姒х姵鐖�
  lv_group_set_default(lv_group_create());

  // 閸掓稑缂撴稉鈧�娑擄�? SDL 濡�鈩冨�?閸ｃ劎鐛ラ崣锟� 娴ｆ粈璐烲VGL 閻ㄥ�?绶�閸忋儴锟芥儳锟斤�?
  lv_display_t * disp = lv_sdl_window_create(w, h);

  //閸掓繂锟藉��瀵叉Η鐘崇垼鐠佹儳锟斤�?
  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());   // 鐠佸墽鐤嗘潏鎾冲弳缂侊拷
  lv_indev_set_display(mouse, disp);                   // 缂佹垵鐣鹃崚鐗堟▔缁�?楦匡拷鎯э拷锟�?
  lv_display_set_default(disp);                        // 鐠佸墽鐤嗘�涙�匡拷銈嗘▔缁�?楦匡拷鎯э拷锟�?

  // 鐠佸墽鐤嗘Η鐘崇垼閸忓��??�?
  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.-瀵�鏇炲弳閸忓��?垼閻ㄥ�??娴橀崓蹇氱カ濠э拷*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor - 閸︺劌鐫嗛獮鏇氱瑐閸掓稑缂撴稉鈧�娑擄拷濞茶�插З鐎电�呰�? */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source - 閸︺劏绻栨稉锟界�?电�呰杽閸愬懘鍎�?崘娆忓弳娑撯偓娑擄拷閸ユ儳鍎氶敍�?ョ炊閺嶅洨锟斤拷婢惰�?绱�*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver - 鐏忓棗鍘滈弽鍥э拷�?呰杽閸滃矂绱堕弽鍥�绻�?�悰宀�?�?︾€癸拷*/

  //姒х姵鐖ｅ�婃俺鐤嗗Ο鈩冨珯
  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  //闁匡拷閻╂�跨翻閸忥�?
  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}
