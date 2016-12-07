#include "pebble.h"


#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ICONS 4 //TODO
#define NUM_FIRST_MENU_ITEMS 4


static Window *s_main_window;
// MENU VARIABLES
static MenuLayer *s_menu_layer;
static GBitmap *s_menu_icons[NUM_MENU_ICONS];
static GBitmap *s_background_bitmap;

static int s_current_icon = 0;

// WEATHER VARIABLES
static TextLayer *s_temperature_layer;
static TextLayer *s_city_layer;
static BitmapLayer *s_icon_layer;
static GBitmap *s_icon_bitmap = NULL;

static AppSync s_sync;
static uint8_t s_sync_buffer[64];

enum WeatherKey {
  WEATHER_ICON_KEY = 0x0,
};

// DEFINE SOME HEADERS
static void request_weather(int device) ;

// MENU METHODS


static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
        // Draw title text in the section header
        menu_cell_basic_header_draw(ctx, cell_layer, "Energenie");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, "Socket 1", "", NULL);
          break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, "Socket 2", "", NULL);
          break;
        case 2:
          menu_cell_basic_draw(ctx, cell_layer, "Socket 3", "", NULL);
          break;
        case 3:
            menu_cell_basic_draw(ctx, cell_layer, "Socket 4", "", NULL);
            break;
      }
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
  APP_LOG(APP_LOG_LEVEL_INFO, "Selected Energenie socket %d", cell_index->row);
  request_weather(cell_index->row);

}

// WEATHER METHODS
static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "sync_tuple_changed_callback: %lu", key);
  switch (key) {
    case WEATHER_ICON_KEY:
    // We don't actually care about the response here;
      APP_LOG (APP_LOG_LEVEL_INFO, "Received callback response: %d", new_tuple->value->uint8);
      break;
  }
}

static void request_weather(int device) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
    // Error creating outbound message
    return;
  }

  //int value = device;
  dict_write_int(iter, device, &device, sizeof(int), true);
  dict_write_end(iter);

  app_message_outbox_send();
}

// COMMON STUFF


static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // MENU STUFF
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = PBL_IF_RECT_ELSE(menu_get_header_height_callback, NULL),
    .draw_header = PBL_IF_RECT_ELSE(menu_draw_header_callback, NULL),
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback
    //.get_cell_height = PBL_IF_ROUND_ELSE(get_cell_height_callback, NULL),
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));

  // WEATHER STUFF
  Tuplet initial_values[] = {
    TupletInteger(WEATHER_ICON_KEY, (uint8_t) 1),
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer),
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

  //request_weather();
}

static void window_unload(Window *window) {
  // MENU STUFF

  menu_layer_destroy(s_menu_layer);
  for (int i = 0; i < NUM_MENU_ICONS; i++) {
    gbitmap_destroy(s_menu_icons[i]);
  }

  // WEATHER STUFF
  if (s_icon_bitmap) {
    gbitmap_destroy(s_icon_bitmap);
  }

  text_layer_destroy(s_city_layer);
  text_layer_destroy(s_temperature_layer);
  bitmap_layer_destroy(s_icon_layer);
}


static void init(void) {
  s_main_window = window_create();
  window_set_background_color(s_main_window, PBL_IF_COLOR_ELSE(GColorIndigo, GColorBlack));
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_main_window, true);

  app_message_open(64, 64);
}

static void deinit(void) {
  window_destroy(s_main_window);

  app_sync_deinit(&s_sync);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
