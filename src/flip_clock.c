#include <pebble.h>
#include "flip_layer.h"
#include "flip_layer_extention.h"
#include "effect_layer.h"
  
EffectLayer *effect_layer_minutes_1;


static AppSync sync;
static uint8_t sync_buffer[256];

static bool appStarted = false;

enum {
  BLUETOOTHVIBE_KEY = 0x0,
  HOURLYVIBE_KEY = 0x1,
  TEMP_KEY = 0x2,
  CONDITION_KEY = 0x3,
  DATEFORMAT1_KEY = 0x4,
  DATEFORMAT2_KEY = 0x5,
  DATEFORMAT3_KEY = 0x6
};

Layer* weather_holder;
TextLayer *temp_layer;
//TextLayer *condition_layer;

static GBitmap *separator_image;
static BitmapLayer *separator_layer;

TextLayer *layer_date_text1;
TextLayer *layer_date_text2;
TextLayer *layer_date_text3;
TextLayer *layer_date_text4;
TextLayer *layer_date_text5;
TextLayer *layer_date_text6;
TextLayer *layer_date_text_a;
TextLayer *layer_date_text_a2;
TextLayer *layer_date_text_b;
TextLayer *layer_date_text_b2;
TextLayer *layer_date_text_c;
TextLayer *layer_date_text_c2;
TextLayer *layer_date_text_d;
TextLayer *layer_date_text_e;
TextLayer *layer_date_text_e2;

static int dateformat1;
static int dateformat2;
static int dateformat3;
static int bluetoothvibe;
static int hourlyvibe;

int cur_day = -1;

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

BitmapLayer *blank_layer;
GBitmap *blank_bitmap;

//static GBitmap *bluetooth_image;
//static BitmapLayer *bluetooth_layer;

static GFont custom_font;


void format () {
	
	  switch (dateformat1) {
  
	case 0:  //day number
		 layer_set_hidden(text_layer_get_layer(layer_date_text2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text3), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text4), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text5), true);
		 
		 layer_set_hidden(text_layer_get_layer(layer_date_text1), false); 
	break;
		  
	case 1: //month number
		 layer_set_hidden(text_layer_get_layer(layer_date_text1), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text3), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text4), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text2), true);
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text5), false); 
	break;
		  
	case 2: // day name
		 layer_set_hidden(text_layer_get_layer(layer_date_text1), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text3), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text4), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text5), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text2), false); 
	break;
		  
	case 3: //short day
		 layer_set_hidden(text_layer_get_layer(layer_date_text1), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text4), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text5), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text3), false); 
	break;
		  
	case 4: //year
		 layer_set_hidden(text_layer_get_layer(layer_date_text1), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text3), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text5), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text4), false); 
	break;
  }
}

void format2 () {
	
	  switch (dateformat2) {
  
	case 0:  //day number
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e), true);
		 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a), false); 
	break;
		  
	case 1: //month number
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e), false); 
	break;
		  
	case 2: // day name
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b), false); 
	break;
		  
	case 3: //short day
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c), false); 
	break;	  

  }
}

void format3 () {
	
	  switch (dateformat3) {
  
	case 0:  //day number
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_d), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e2), true);
		 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a2), false); 
	break;
		  
	case 1: //month number
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_d), true);
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e2), false); 
	break;
		  
	case 2: // day name
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_d), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e2), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b2), false); 
	break;
		  
	case 3: //short day
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_d), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e2), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c2), false); 
	break;
		  
	case 4: //year
		 layer_set_hidden(text_layer_get_layer(layer_date_text_a2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_b2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_c2), true); 
		 layer_set_hidden(text_layer_get_layer(layer_date_text_e2), true); 
		  
		 layer_set_hidden(text_layer_get_layer(layer_date_text_d), false); 
	break;
  }
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
	  
    case BLUETOOTHVIBE_KEY:
      bluetoothvibe = new_tuple->value->uint8 != 0;
	  persist_write_bool(BLUETOOTHVIBE_KEY, bluetoothvibe);
      break;      
	  
    case HOURLYVIBE_KEY:
      hourlyvibe = new_tuple->value->uint8 != 0;
	  persist_write_bool(HOURLYVIBE_KEY, hourlyvibe);	
      break;
	  
	case TEMP_KEY:
      text_layer_set_text(temp_layer, new_tuple->value->cstring);
      break;
	  
	case CONDITION_KEY:
    //  text_layer_set_text(condition_layer, new_tuple->value->cstring);
      break;
	  
 	case DATEFORMAT1_KEY:
      dateformat1 = new_tuple->value->uint8;
	  persist_write_bool(DATEFORMAT1_KEY, dateformat1);
	//  format();
  	break;

	case DATEFORMAT2_KEY:
      dateformat2 = new_tuple->value->uint8;
	  persist_write_bool(DATEFORMAT2_KEY, dateformat2);
	//  format2();
  	break;
	  
 	case DATEFORMAT3_KEY:
      dateformat3 = new_tuple->value->uint8;
	  persist_write_bool(DATEFORMAT3_KEY, dateformat3);
	//  format3();
  	break;
  }
}

unsigned short get_display_hour(unsigned short hour) {
  if (clock_is_24h_style()) {
    return hour;
  }
  unsigned short display_hour = hour % 12;
  // Converts "0" to "12"
  return display_hour ? display_hour : 12;
	
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  
    static char date_text[] = "00";
    static char date_text2[] = "AUG";
    static char date_text3[] = "MON";
 //   static char date_text4[] = "2015";
    static char date_text5[] = "00";

    
//    char *h_time_format;

//day num
	  strftime(date_text, sizeof(date_text), "%d", tick_time);
	  text_layer_set_text(layer_date_text1, date_text);
	  text_layer_set_text(layer_date_text_a, date_text);
	  text_layer_set_text(layer_date_text_a2, date_text);
//short month	
	  strftime(date_text2, sizeof(date_text2), "%b", tick_time);
	  text_layer_set_text(layer_date_text2, date_text2);
	  text_layer_set_text(layer_date_text_b, date_text2);
	  text_layer_set_text(layer_date_text_b2, date_text2);

//short day
	  strftime(date_text3, sizeof(date_text3), "%a", tick_time);
	  text_layer_set_text(layer_date_text3, date_text3);
	  text_layer_set_text(layer_date_text_c, date_text3);
	  text_layer_set_text(layer_date_text_c2, date_text3);

//yyyy
	//  strftime(date_text4, sizeof(date_text4), "%Y", tick_time);
	//  text_layer_set_text(layer_date_text4, date_text4); 
	//  text_layer_set_text(layer_date_text_d, date_text4); 

//month num
	  strftime(date_text5, sizeof(date_text5), "%m", tick_time);
	  text_layer_set_text(layer_date_text5, date_text5); 
	  text_layer_set_text(layer_date_text_e, date_text5); 
	  text_layer_set_text(layer_date_text_e2, date_text5); 
	
	
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

static void toggle_bluetooth_icon(bool connected) {
  if(appStarted && !connected && bluetoothvibe) {
    //vibe!
    vibes_long_pulse();
  }
//  layer_set_hidden(bitmap_layer_get_layer(bluetooth_layer), !connected);
}

void bluetooth_connection_callback(bool connected) {
  toggle_bluetooth_icon(connected);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
	  GRect bounds = layer_get_bounds(window_layer);
	
  custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUSTOM_18));

  temp_layer  = text_layer_create(GRect(1, 128, 32, 30));
  text_layer_set_text_color(temp_layer, GColorYellow);
  text_layer_set_background_color(temp_layer, GColorClear);
  text_layer_set_font(temp_layer, custom_font);
  text_layer_set_text_alignment(temp_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(temp_layer));
/*	
  condition_layer  = text_layer_create(GRect(40, 121, 100, 30));
  text_layer_set_text_color(condition_layer, color_dow);
  text_layer_set_background_color(condition_layer, GColorClear);
  text_layer_set_font(condition_layer, custom_font);
  text_layer_set_text_alignment(condition_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(condition_layer));
*/
    // date format layer position and alignment
	
// day number
    layer_date_text1 = text_layer_create(GRect(0, 16, 140, 20));
	text_layer_set_text_color(layer_date_text1, GColorYellow);		
    text_layer_set_background_color(layer_date_text1, GColorClear);
    text_layer_set_font(layer_date_text1, custom_font);
    text_layer_set_text_alignment(layer_date_text1, GTextAlignmentRight);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text1));
	//layer_set_hidden(text_layer_get_layer(layer_date_text1), true); 

	   layer_date_text_a = text_layer_create(GRect(76, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_a, GColorYellow);		
    text_layer_set_background_color(layer_date_text_a, GColorClear);
    text_layer_set_font(layer_date_text_a, custom_font);
    text_layer_set_text_alignment(layer_date_text_a, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_a));
	layer_set_hidden(text_layer_get_layer(layer_date_text_a), true); 
	
	   layer_date_text_a2 = text_layer_create(GRect(111, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_a2, GColorYellow);		
    text_layer_set_background_color(layer_date_text_a2, GColorClear);
    text_layer_set_font(layer_date_text_a2, custom_font);
    text_layer_set_text_alignment(layer_date_text_a2, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_a2));
	layer_set_hidden(text_layer_get_layer(layer_date_text_a2), true); 
	
// month short
    layer_date_text2 = text_layer_create(GRect(80, 16, 40, 20));
	text_layer_set_text_color(layer_date_text2, GColorYellow);		
    text_layer_set_font(layer_date_text2, custom_font);
    text_layer_set_background_color(layer_date_text2, GColorClear);
    text_layer_set_text_alignment(layer_date_text2, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text2));
	//layer_set_hidden(text_layer_get_layer(layer_date_text2), true); 
	
	    layer_date_text_b = text_layer_create(GRect(76, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_b, GColorBlack);		
    text_layer_set_font(layer_date_text_b, custom_font);
    text_layer_set_background_color(layer_date_text_b, GColorClear);
    text_layer_set_text_alignment(layer_date_text_b, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_b));
	layer_set_hidden(text_layer_get_layer(layer_date_text_b), true); 
	
	    layer_date_text_b2 = text_layer_create(GRect(111, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_b2, GColorYellow);		
    text_layer_set_font(layer_date_text_b2, custom_font);
    text_layer_set_background_color(layer_date_text_b2, GColorClear);
    text_layer_set_text_alignment(layer_date_text_b2, GTextAlignmentLeft);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_b2));
	layer_set_hidden(text_layer_get_layer(layer_date_text_b2), true); 
	
//day short
    layer_date_text3 = text_layer_create(GRect(8, 16, 100, 20));
	text_layer_set_text_color(layer_date_text3, GColorYellow);		
    text_layer_set_background_color(layer_date_text3, GColorClear);
    text_layer_set_font(layer_date_text3, custom_font);
    text_layer_set_text_alignment(layer_date_text3, GTextAlignmentLeft);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text3));
	//layer_set_hidden(text_layer_get_layer(layer_date_text3), true); 

	    layer_date_text_c = text_layer_create(GRect(76, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_c, GColorYellow);		
    text_layer_set_background_color(layer_date_text_c, GColorClear);
    text_layer_set_font(layer_date_text_c, custom_font);
    text_layer_set_text_alignment(layer_date_text_c, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_c));
	layer_set_hidden(text_layer_get_layer(layer_date_text_c), true); 
	
	
	    layer_date_text_c2 = text_layer_create(GRect(110, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_c2, GColorYellow);		
    text_layer_set_background_color(layer_date_text_c2, GColorClear);
    text_layer_set_font(layer_date_text_c2, custom_font);
    text_layer_set_text_alignment(layer_date_text_c2, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_c2));
	layer_set_hidden(text_layer_get_layer(layer_date_text_c2), true); 
/*	
//year
    layer_date_text4 = text_layer_create(GRect(60, 16, 60, 20));
	text_layer_set_text_color(layer_date_text4, GColorYellow);		
    text_layer_set_background_color(layer_date_text4, GColorClear);
    text_layer_set_font(layer_date_text4, custom_font);
    text_layer_set_text_alignment(layer_date_text4, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text4));
	//layer_set_hidden(text_layer_get_layer(layer_date_text4), true); 

    layer_date_text_d = text_layer_create(GRect(111, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_d, GColorBlack);		
    text_layer_set_background_color(layer_date_text_d, GColorClear);
    text_layer_set_font(layer_date_text_d, custom_font);
    text_layer_set_text_alignment(layer_date_text_d, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_d));
	layer_set_hidden(text_layer_get_layer(layer_date_text_d), true); 
	*/
//month number
    layer_date_text5 = text_layer_create(GRect(40, 20, 31, 40));
	text_layer_set_text_color(layer_date_text5, GColorBlack);		
    text_layer_set_background_color(layer_date_text5, GColorClear);
    text_layer_set_font(layer_date_text5, custom_font);
    text_layer_set_text_alignment(layer_date_text5, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text5));
	layer_set_hidden(text_layer_get_layer(layer_date_text5), true); 
	
    layer_date_text_e = text_layer_create(GRect(76, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_e, GColorBlack);		
    text_layer_set_background_color(layer_date_text_e, GColorClear);
    text_layer_set_font(layer_date_text_e, custom_font);
    text_layer_set_text_alignment(layer_date_text_e, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_e));
	layer_set_hidden(text_layer_get_layer(layer_date_text_e), true); 
	
    layer_date_text_e2 = text_layer_create(GRect(111, 20, 31, 40));
	text_layer_set_text_color(layer_date_text_e2, GColorBlack);		
    text_layer_set_background_color(layer_date_text_e2, GColorClear);
    text_layer_set_font(layer_date_text_e2, custom_font);
    text_layer_set_text_alignment(layer_date_text_e2, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(layer_date_text_e2));
	layer_set_hidden(text_layer_get_layer(layer_date_text_e2), true); 
	
#ifdef PBL_PLATFORM_CHALK
    layer[0] = flip_layer_create(GRect(20 , (180-90)/2, 32, 90));
    layer[1] = flip_layer_create(GRect(54 , (180-90)/2, 32, 90));
    layer[2] = flip_layer_create(GRect(98  , (180-90)/2, 32, 90));
    layer[3] = flip_layer_create(GRect(132  , (180-90)/2, 32, 90));
#else
    layer[0] = flip_layer_create(GRect(0 , (168-90)/2, 32, 90));
    layer[1] = flip_layer_create(GRect(34 , (168-90)/2, 32, 90));
    layer[2] = flip_layer_create(GRect(78  , (168-90)/2, 32, 90));
    layer[3] = flip_layer_create(GRect(112  , (168-90)/2, 32, 90));
#endif
	
	
  for(int i=0; i<4; i++){
    flip_layer_set_images(layer[i], NUMBER_IMAGE_RESOURCE_UP_IDS, NUMBER_IMAGE_RESOURCE_DOWN_IDS, NUMBER_IMAGE_COUNT);
    layer_add_child(window_layer, flip_layer_get_layer(layer[i]));
  }

  blank_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLANK);
  blank_layer = bitmap_layer_create(GRect(0, 39, 32, 90));
  bitmap_layer_set_bitmap(blank_layer, blank_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(blank_layer));	
  layer_set_hidden(bitmap_layer_get_layer(blank_layer), true);

	
	
  separator_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SEPARATOR);
  GRect bitmap_bounds = gbitmap_get_bounds(separator_image);
	
#ifdef PBL_PLATFORM_CHALK
  GRect frame = GRect(81, 33, bitmap_bounds.size.w, bitmap_bounds.size.h);
#else
  GRect frame = GRect(67, 80, bitmap_bounds.size.w, bitmap_bounds.size.h);
#endif
  separator_layer = bitmap_layer_create(frame);
  bitmap_layer_set_bitmap(separator_layer, separator_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(separator_layer));   
	
	
	

  // creating effect layer to rotate minutes
  effect_layer_minutes_1 = effect_layer_create( bounds );

//#ifdef PBL_PLATFORM_CHALK
//  effect_layer_minutes_1 = effect_layer_create(GRect(0,0,180,180));
  effect_layer_add_effect(effect_layer_minutes_1, effect_rotate_90_degrees, (void *)true);
  layer_add_child(window_layer, effect_layer_get_layer(effect_layer_minutes_1));
//#else
//  effect_layer_minutes_1 = effect_layer_create(GRect(-1,0,145,168));
//  effect_layer_add_effect(effect_layer_minutes_1, effect_rotate_90_degrees, (void *)true);
//  layer_add_child(window_layer, effect_layer_get_layer(effect_layer_minutes_1));	
	
//#endif
	
}

static void window_unload(Window *window) {
	
  for(int i=0; i<4; i++){
    flip_layer_destroy(layer[i]);
  }
	
//  bitmap_layer_destroy(bg_layer);
  bitmap_layer_destroy(blank_layer);
//  bitmap_layer_destroy(bluetooth_layer);
  text_layer_destroy(temp_layer);
//  text_layer_destroy(condition_layer);
  
text_layer_destroy(layer_date_text1);
text_layer_destroy(layer_date_text2);
text_layer_destroy(layer_date_text3);
text_layer_destroy(layer_date_text4);
text_layer_destroy(layer_date_text5);
text_layer_destroy(layer_date_text6);
text_layer_destroy(layer_date_text_a);
text_layer_destroy(layer_date_text_a2);
text_layer_destroy(layer_date_text_b);
text_layer_destroy(layer_date_text_b2);
text_layer_destroy(layer_date_text_c);
text_layer_destroy(layer_date_text_c2);
text_layer_destroy(layer_date_text_d);
text_layer_destroy(layer_date_text_e);
text_layer_destroy(layer_date_text_e2);
	
	
//  gbitmap_destroy(bg_bitmap);
  gbitmap_destroy(blank_bitmap);
//  gbitmap_destroy(bluetooth_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(separator_layer));
  bitmap_layer_destroy(separator_layer);
  gbitmap_destroy(separator_image);


}

static void init(void) {

  const int inbound_size = 256;
  const int outbound_size = 256;
  app_message_open(inbound_size, outbound_size);  
	
// initializing colors
#ifdef PBL_COLOR
   color_back = GColorBlack;
   color_date = GColorBlack;
   color_dow =  GColorWhite ;
   color_battery = GColorBlack;
   color_bluetooth = GColorBlack;
  
  digit_back = GColorBlack ;
  digit_img = GColorWhite;
  
#else
   color_back = GColorBlack;
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

    TupletInteger(BLUETOOTHVIBE_KEY, persist_read_bool(BLUETOOTHVIBE_KEY)),
    TupletInteger(HOURLYVIBE_KEY, persist_read_bool(HOURLYVIBE_KEY)),
	TupletCString(CONDITION_KEY, ""),
	TupletCString(TEMP_KEY, ""),	  
    TupletInteger(DATEFORMAT1_KEY, persist_read_bool(DATEFORMAT1_KEY)),
    TupletInteger(DATEFORMAT2_KEY, persist_read_bool(DATEFORMAT2_KEY)),
    TupletInteger(DATEFORMAT3_KEY, persist_read_bool(DATEFORMAT3_KEY)),
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
