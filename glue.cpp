
#include <emscripten.h>
#include <stdlib.h>

EM_JS_DEPS(webidl_binder, "$intArrayFromString,$UTF8ToString,$alignMemory,$addOnInit");

extern "C" {

// Define custom allocator functions that we can force export using
// EMSCRIPTEN_KEEPALIVE.  This avoids all webidl users having to add
// malloc/free to -sEXPORTED_FUNCTIONS.
EMSCRIPTEN_KEEPALIVE void webidl_free(void* p) { free(p); }
EMSCRIPTEN_KEEPALIVE void* webidl_malloc(size_t len) { return malloc(len); }


// Interface: VoidPtr


void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// Interface: Pilot


Pilot* EMSCRIPTEN_KEEPALIVE emscripten_bind_Pilot_Pilot_0() {
  return new Pilot();
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_Pilot_get_latitude_0(Pilot* self) {
  return self->get_latitude();
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_Pilot_get_longitude_0(Pilot* self) {
  return self->get_longitude();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Pilot_set_latitude_1(Pilot* self, float lat) {
  self->set_latitude(lat);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Pilot_set_longitude_1(Pilot* self, float lng) {
  self->set_longitude(lng);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Pilot_set_destination_2(Pilot* self, float lat, float lng) {
  self->set_destination(lat, lng);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Pilot_start_mission_1(Pilot* self, char* mission_name) {
  self->start_mission(mission_name);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Pilot___destroy___0(Pilot* self) {
  delete self;
}

// Interface: Missions


Missions* EMSCRIPTEN_KEEPALIVE emscripten_bind_Missions_Missions_0() {
  return new Missions();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Missions_start_mission_1(Missions* self, char* mission_name) {
  self->start_mission(mission_name);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Missions___destroy___0(Missions* self) {
  delete self;
}

// Interface: Coordinates


Coordinates* EMSCRIPTEN_KEEPALIVE emscripten_bind_Coordinates_Coordinates_0() {
  return new Coordinates();
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_Coordinates_get_latitiude_0(Coordinates* self) {
  return self->get_latitiude();
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_Coordinates_get_longitude_0(Coordinates* self) {
  return self->get_longitude();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Coordinates_set_latitude_1(Coordinates* self, float lat) {
  self->set_latitude(lat);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Coordinates_set_longitude_1(Coordinates* self, float lng) {
  self->set_longitude(lng);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Coordinates_set_destination_2(Coordinates* self, float lat, float lng) {
  self->set_destination(lat, lng);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Coordinates___destroy___0(Coordinates* self) {
  delete self;
}

}

