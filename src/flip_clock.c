#include <pebble.h>
#include "flip_layer.h"
#include "flip_layer_extention.h"
#include "effec_layer.h"
  
static AppSync sync;
static uint8_t sync_buffer[128];

static bool appStarted = false;

enum {
  BLUETOOTHVIBE_KEY = 0x0,
  HOURLYVIBE_KEY = 0x1,
  TEMP_KEY = 0x2,
  CONDITION_KEY = 0x3,
};

Layer* weather_holder;
TextLayer *temp_layer;
TextLayer *condition_layer;

static int bluetoothvibe;
static int hourlyvibe;

#define NUMBER_IMAGE_COUNT 10

int NUMBER_IMAGE_RESOURCE_UP_IDS[NUMBER_IMAGE_COUNT] = {
  RESOURCE_ID_IMAGE_0_UP,
  RESOURCE_ID_IMAGE_1_UP,
  RESOURCE_ID_IMAGE_2_UP,
  RESOURCE_ID_IMAGE_3_UP,
  RESOURCE_ID_IMAGE_4_UP,
  RESOURCE_ID_IMAGE_5_UP,
  RESOURCE_ID_IMAGE_6_UP,
  RESOURCE_ID_IMAGE_7_UP,
  RESOURCE_ID_IMAGE_8_UP,
  RESOURCE_ID_IMAGE_9_UP,
};

int NUMBER_IMAGE_RESOURCE_DOWN_IDS[NUMBER_IMAGE_COUNT] = {
  RESOURCE_ID_IMAGE_0_DOWN,
  RESOURCE_ID_IMAGE_1_DOWN,
  RESOURCE_ID_IMAGE_2_DOWN,
  RESOURCE_ID_IMAGE_3_DOWN,
  RESOURCE_ID_IMAGE_4_DOWN,
  RESOURCE_ID_IMAGE_5_DOWN,
  RESOURCE_ID_IMAGE_6_DOWN,
  RESOURCE_ID_IMAGE_7_DOWN,
  RESOURCE_ID_IMAGE_8_DOWN,
  RESOURCE_ID_IMAGE_9_DOWN,
};  

static Window *window;
static FlipLayer *layer[4];

static TextLayer *text_layer_date;
char buffer_date[] = "SEP      31";

static TextLayer *text_layer_dow;
char buffer_dow[] = "SAT";

BitmapLayer *bg_layer;
GBitmap *bg_bitmap;

BitmapLayer *blank_layer;
GBitmap *blank_bitmap;

static GBitmap *bluetooth_image;
static BitmapLayer *bluetooth_layer;

static GFont *custom_font;


unsigned short get_display_hour(unsigned short hour) {
  if (clock_is_24h_style()) {
    return hour;
  }
  unsigned short display_hour = hour % 12;
  // Converts "0" to "12"
  return display_hour ? display_hour : 12;
	
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  
  strftime(buffer_date, sizeof("SEP      31"), "%b      %d", tick_time);
  text_layer_set_text(text_layer_date, buffer_date);
  
  strftime(buffer_dow, sizeof("SAT"), "%a", tick_time);
  text_layer_set_text(text_layer_dow, buffer_dow);
  
  if (!clock_is_24h_style()) {
    
      if( tick_time->tm_hour > 11 ) {   // YG Jun-25-2014: 0..11 - am 12..23 - pm
       //   strcat(buffer_dow, " PM" );
          if( tick_time->tm_hour > 12 ) tick_time->tm_hour -= 12;
      } else {
       //   strcat(buffer_dow, " AM" );
          if( tick_time->tm_hour == 0 ) tick_time->tm_hour = 12;
      }        
  } 
  
  flip_layer_animate_to(layer[0], tick_time->tm_hour / 10);
  flip_layer_animate_to(layer[1], tick_time->tm_hour % 10);
  flip_layer_animate_to(layer[2], tick_time->tm_min / 10);
  flip_layer_animate_to(layer[3], tick_time->tm_min % 10);
 	  
  unsigned short display_hour = get_display_hour(tick_time->tm_hour);

  if (!clock_is_24h_style()) {
    
    if (display_hour/10 == 0) {
      layer_set_hidden(bitmap_layer_get_layer(blank_layer), false);
    }
    else {
      layer_set_hidden(bitmap_layer_get_layer(blank_layer), true);
    }
  }     
}


static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
	  
	case CONDITION_KEY:
      text_layer_set_text(condition_layer, new_tuple->value->cstring);
      break;
	  
	case TEMP_KEY:
      text_layer_set_text(temp_layer, new_tuple->value->cstring);
      break;
	  
    case BLUETOOTHVIBE_KEY:
      bluetoothvibe = new_tuple->value->uint8 != 0;
	  persist_write_bool(BLUETOOTHVIBE_KEY, bluetoothvibe);
      break;      
	  
    case HOURLYVIBE_KEY:
      hourlyvibe = new_tuple->value->uint8 != 0;
	  persist_write_bool(HOURLYVIBE_KEY, hourlyvibe);	
      break;	  
  }
}

static void toggle_bluetooth_icon(bool connected) {
  if(appStarted && !connected && bluetoothvibe) {
    //vibe!
    vibes_long_pulse();
  }
  layer_set_hidden(bitmap_layer_get_layer(bluetooth_layer), !connected);
}

void bluetooth_connection_callback(bool connected) {
  toggle_bluetooth_icon(connected);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WOODBG);
  bg_layer = bitmap_layer_create(GRect(0, -1, 144, 168));
  bitmap_layer_set_bitmap(bg_layer, bg_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(bg_layer));	
	
  bluetooth_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH);
#ifdef PBL_PLATFORM_BASALT
  GRect bitmap_bounds = gbitmap_get_bounds(bluetooth_image);
#else
  GRect bitmap_bounds = bluetooth_image->bounds;
#endif
  GRect frame = GRect(0, 20, bitmap_bounds.size.w, bitmap_bounds.size.h);

  bluetooth_layer = bitmap_layer_create(frame);
  bitmap_layer_set_bitmap(bluetooth_layer, bluetooth_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));	
	
  custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUSTOM_22));
	
  text_layer_date = text_layer_create(GRect(76, 20, 90, 40));
  text_layer_set_text_color(text_layer_date, color_date);
  text_layer_set_background_color(text_layer_date, GColorClear);
  text_layer_set_text_alignment(text_layer_date, GTextAlignmentLeft);
  text_layer_set_font(text_layer_date, custom_font);
  layer_add_child(window_layer, text_layer_get_layer(text_layer_date));
  
  text_layer_dow = text_layer_create(GRect(40, 20, 90, 40));
  text_layer_set_text_color(text_layer_dow, color_dow);
  text_layer_set_background_color(text_layer_dow, GColorClear);
  text_layer_set_text_alignment(text_layer_dow, GTextAlignmentLeft);
  text_layer_set_font(text_layer_dow, custom_font);
  layer_add_child(window_layer, text_layer_get_layer(text_layer_dow));
    
  temp_layer  = text_layer_create(GRect(2, 121, 32, 30));
  text_layer_set_text_color(temp_layer, color_dow);
  text_layer_set_background_color(temp_layer, GColorClear);
  text_layer_set_font(temp_layer, custom_font);
  text_layer_set_text_alignment(temp_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(temp_layer));
	
  condition_layer  = text_layer_create(GRect(40, 121, 100, 30));
  text_layer_set_text_color(condition_layer, color_dow);
  text_layer_set_background_color(condition_layer, GColorClear);
  text_layer_set_font(condition_layer, custom_font);
  text_layer_set_text_alignment(condition_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(condition_layer));
	

  for(int i=0; i<4; i++){
    layer[i] = flip_layer_create(GRect(36 * i + (i>1? 1:0) , (168-66)/2, 32, 66));
  }
  
  for(int i=0; i<4; i++){
    flip_layer_set_images(layer[i], NUMBER_IMAGE_RESOURCE_UP_IDS, NUMBER_IMAGE_RESOURCE_DOWN_IDS, NUMBER_IMAGE_COUNT);
    layer_add_child(window_layer, flip_layer_get_layer(layer[i]));
  }

  blank_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLANK);
  blank_layer = bitmap_layer_create(GRect(1, 52, 33, 66));
  bitmap_layer_set_bitmap(blank_layer, blank_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(blank_layer));	
  layer_set_hidden(bitmap_layer_get_layer(blank_layer), true);

}

static void window_unload(Window *window) {
	
  for(int i=0; i<4; i++){
    flip_layer_destroy(layer[i]);
  }
	
  bitmap_layer_destroy(bg_layer);
  bitmap_layer_destroy(blank_layer);
  bitmap_layer_destroy(bluetooth_layer);
  text_layer_destroy(text_layer_date);
  text_layer_destroy(text_layer_dow);
  text_layer_destroy(temp_layer);
  text_layer_destroy(condition_layer);
  
  gbitmap_destroy(bg_bitmap);
  gbitmap_destroy(blank_bitmap);
  gbitmap_destroy(bluetooth_image);

}

static void init(void) {

  const int inbound_size = 128;
  const int outbound_size = 128;
  app_message_open(inbound_size, outbound_size);  
	
// initializing colors
#ifdef PBL_COLOR
   color_back = GColorBlack;
   color_date = GColorBlack;
   color_dow =  GColorBlack ;
   color_battery = GColorBlack;
   color_bluetooth = GColorBlack;
  
  digit_back = GColorBlack ;
  digit_img = GColorWhite;
  
#else
   color_back = GColorWhite;
   color_date = GColorBlack;
   color_dow =  GColorBlack;
   color_battery = GColorBlack;
   color_bluetooth = GColorBlack;
  
  digit_back = GColorBlack;
  digit_img = GColorWhite;
  
#endif
  
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  window_set_background_color(window, color_back);
  window_stack_push(window, true);

	
  Tuplet initial_values[] = {
	TupletCString(CONDITION_KEY, ""),
	TupletCString(TEMP_KEY, ""),
    TupletInteger(BLUETOOTHVIBE_KEY, persist_read_bool(BLUETOOTHVIBE_KEY)),
    TupletInteger(HOURLYVIBE_KEY, persist_read_bool(HOURLYVIBE_KEY)),
  };
  
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
  sync_tuple_changed_callback, NULL, NULL);
   
  appStarted = true;

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
   
  bluetooth_connection_service_subscribe(bluetooth_connection_callback);
}

static void deinit(void) {
  app_sync_deinit(&sync);
  tick_timer_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
	
  fonts_unload_custom_font(custom_font);

  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
