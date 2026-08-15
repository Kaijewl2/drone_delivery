
#include <emscripten.h>
#include <stdlib.h>

EM_JS_DEPS(webidl_binder,
           "$intArrayFromString,$UTF8ToString,$alignMemory,$addOnInit");

extern "C" {

// Define custom allocator functions that we can force export using
// EMSCRIPTEN_KEEPALIVE.  This avoids all webidl users having to add
// malloc/free to -sEXPORTED_FUNCTIONS.
EMSCRIPTEN_KEEPALIVE void webidl_free(void *p) { free(p); }
EMSCRIPTEN_KEEPALIVE void *webidl_malloc(size_t len) { return malloc(len); }

// Interface: VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void **self) {
  delete self;
}

// Interface: Foo

Foo *EMSCRIPTEN_KEEPALIVE emscripten_bind_Foo_Foo_0() { return new Foo(); }

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Foo_getVal_0(Foo *self) {
  return self->getVal();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Foo_setVal_1(Foo *self, int v) {
  self->setVal(v);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Foo___destroy___0(Foo *self) {
  delete self;
}

// Interface: Bar

Bar *EMSCRIPTEN_KEEPALIVE emscripten_bind_Bar_Bar_1(int val) {
  return new Bar(val);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Bar_doSomething_0(Bar *self) {
  self->doSomething();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Bar___destroy___0(Bar *self) {
  delete self;
}
}
