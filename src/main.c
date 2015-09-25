#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_date_font;

static void update_time() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Enter update_time()");

  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create long-lived buffers
  static char time_buffer[] = "XX:XX";
  static char date_buffer[] = "XXXX-XX-XX";
  static char wday_buffer[] = "Wochentag?";

  APP_LOG(APP_LOG_LEVEL_INFO, "--1---");

  // Write the current date, hours, minutes into the buffer
  strftime(date_buffer, sizeof("XXXX-XX-XX"), "%Y-%m-%d", tick_time);
  strftime(date_buffer, sizeof("Wochentag?"), "%A", tick_time);
  if(clock_is_24h_style()) {
    // Use 24 hour format
    strftime(time_buffer, sizeof("XX:XX"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(time_buffer, sizeof("XX:XX"), "%I:%M", tick_time);
  }
  APP_LOG(APP_LOG_LEVEL_INFO, wday_buffer);
  APP_LOG(APP_LOG_LEVEL_INFO, date_buffer);
  APP_LOG(APP_LOG_LEVEL_INFO, time_buffer);

  // Display date and time on the TextLayers
  text_layer_set_text(s_time_layer, time_buffer);
  text_layer_set_text(s_date_layer, date_buffer);
  text_layer_set_text(s_wday_layer, wday_buffer);
  APP_LOG(APP_LOG_LEVEL_INFO, "Leave update_time()");
}

static void main_window_load(Window *window) {
  // Create time and date TextLayers
  s_time_layer = text_layer_create(GRect(0,  0, 144, 60));
  s_date_layer = text_layer_create(GRect(0, 60, 144, 60));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorClear);
#ifdef PBL_COLOR
  text_layer_set_text_color(s_time_layer, GColorDukeBlue);
  text_layer_set_text_color(s_date_layer, GColorDukeBlue);
#else
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_date_layer, GColorBlack);
#endif
  
  // Create GFonts and apply it to TextLayers
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GCE_DRUCKSCHRIFT_55));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GCE_DRUCKSCHRIFT_25));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_date_layer, s_date_font);
  
  // Set alignment to center
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add as a child layers to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
  // Destroy layers and bitmaps
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  
  // Unload fonts
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
    
  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
    // Destroy main Window
    window_destroy(s_main_window);
}

int main(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Enter main()");
  init();
  APP_LOG(APP_LOG_LEVEL_INFO, "Start event loop");
  app_event_loop();
  APP_LOG(APP_LOG_LEVEL_INFO, "Event loop finished");
  deinit();
}