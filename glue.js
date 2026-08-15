
// Bindings utilities

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_webidl_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_webidl_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_webidl_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = alignMemory(len, 8); // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, next time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_webidl_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
};

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    for (var i = 0; i < intArray.length; i++) {
      HEAP8[offset + i] = intArray[i];
    }
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    for (var i = 0; i < value.length; i++) {
      HEAP8[offset + i] = value[i];
    }
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    var heapOffset = offset / 2;
    for (var i = 0; i < value.length; i++) {
      HEAP16[heapOffset + i] = value[i];
    }
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    var heapOffset = offset / 4;
    for (var i = 0; i < value.length; i++) {
      HEAP32[heapOffset + i] = value[i];
    }
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    var heapOffset = offset / 4;
    for (var i = 0; i < value.length; i++) {
      HEAPF32[heapOffset + i] = value[i];
    }
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    var heapOffset = offset / 8;
    for (var i = 0; i < value.length; i++) {
      HEAPF64[heapOffset + i] = value[i];
    }
    return offset;
  }
  return value;
}

// Interface: VoidPtr

/** @suppress {undefinedVars, duplicate} @this{Object} */
function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

/** @suppress {undefinedVars, duplicate} @this{Object} */
VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};

// Interface: Pilot

/** @suppress {undefinedVars, duplicate} @this{Object} */
function Pilot() {
  this.ptr = _emscripten_bind_Pilot_Pilot_0();
  getCache(Pilot)[this.ptr] = this;
};

Pilot.prototype = Object.create(WrapperObject.prototype);
Pilot.prototype.constructor = Pilot;
Pilot.prototype.__class__ = Pilot;
Pilot.__cache__ = {};
Module['Pilot'] = Pilot;
/** @suppress {undefinedVars, duplicate} @this{Object} */
Pilot.prototype['get_latitude'] = Pilot.prototype.get_latitude = function() {
  var self = this.ptr;
  return _emscripten_bind_Pilot_get_latitude_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Pilot.prototype['get_longitude'] = Pilot.prototype.get_longitude = function() {
  var self = this.ptr;
  return _emscripten_bind_Pilot_get_longitude_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Pilot.prototype['set_latitude'] = Pilot.prototype.set_latitude = function(lat) {
  var self = this.ptr;
  if (lat && typeof lat === 'object') lat = lat.ptr;
  _emscripten_bind_Pilot_set_latitude_1(self, lat);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Pilot.prototype['set_longitude'] = Pilot.prototype.set_longitude = function(lng) {
  var self = this.ptr;
  if (lng && typeof lng === 'object') lng = lng.ptr;
  _emscripten_bind_Pilot_set_longitude_1(self, lng);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Pilot.prototype['set_destination'] = Pilot.prototype.set_destination = function(lat, lng) {
  var self = this.ptr;
  if (lat && typeof lat === 'object') lat = lat.ptr;
  if (lng && typeof lng === 'object') lng = lng.ptr;
  _emscripten_bind_Pilot_set_destination_2(self, lat, lng);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Pilot.prototype['start_mission'] = Pilot.prototype.start_mission = function(mission_name) {
  var self = this.ptr;
  ensureCache.prepare();
  if (mission_name && typeof mission_name === 'object') mission_name = mission_name.ptr;
  else mission_name = ensureString(mission_name);
  _emscripten_bind_Pilot_start_mission_1(self, mission_name);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
Pilot.prototype['__destroy__'] = Pilot.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_Pilot___destroy___0(self);
};

// Interface: Missions

/** @suppress {undefinedVars, duplicate} @this{Object} */
function Missions() {
  this.ptr = _emscripten_bind_Missions_Missions_0();
  getCache(Missions)[this.ptr] = this;
};

Missions.prototype = Object.create(WrapperObject.prototype);
Missions.prototype.constructor = Missions;
Missions.prototype.__class__ = Missions;
Missions.__cache__ = {};
Module['Missions'] = Missions;
/** @suppress {undefinedVars, duplicate} @this{Object} */
Missions.prototype['start_mission'] = Missions.prototype.start_mission = function(mission_name) {
  var self = this.ptr;
  ensureCache.prepare();
  if (mission_name && typeof mission_name === 'object') mission_name = mission_name.ptr;
  else mission_name = ensureString(mission_name);
  _emscripten_bind_Missions_start_mission_1(self, mission_name);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
Missions.prototype['__destroy__'] = Missions.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_Missions___destroy___0(self);
};

// Interface: Coordinates

/** @suppress {undefinedVars, duplicate} @this{Object} */
function Coordinates() {
  this.ptr = _emscripten_bind_Coordinates_Coordinates_0();
  getCache(Coordinates)[this.ptr] = this;
};

Coordinates.prototype = Object.create(WrapperObject.prototype);
Coordinates.prototype.constructor = Coordinates;
Coordinates.prototype.__class__ = Coordinates;
Coordinates.__cache__ = {};
Module['Coordinates'] = Coordinates;
/** @suppress {undefinedVars, duplicate} @this{Object} */
Coordinates.prototype['get_latitiude'] = Coordinates.prototype.get_latitiude = function() {
  var self = this.ptr;
  return _emscripten_bind_Coordinates_get_latitiude_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Coordinates.prototype['get_longitude'] = Coordinates.prototype.get_longitude = function() {
  var self = this.ptr;
  return _emscripten_bind_Coordinates_get_longitude_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Coordinates.prototype['set_latitude'] = Coordinates.prototype.set_latitude = function(lat) {
  var self = this.ptr;
  if (lat && typeof lat === 'object') lat = lat.ptr;
  _emscripten_bind_Coordinates_set_latitude_1(self, lat);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Coordinates.prototype['set_longitude'] = Coordinates.prototype.set_longitude = function(lng) {
  var self = this.ptr;
  if (lng && typeof lng === 'object') lng = lng.ptr;
  _emscripten_bind_Coordinates_set_longitude_1(self, lng);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Coordinates.prototype['set_destination'] = Coordinates.prototype.set_destination = function(lat, lng) {
  var self = this.ptr;
  if (lat && typeof lat === 'object') lat = lat.ptr;
  if (lng && typeof lng === 'object') lng = lng.ptr;
  _emscripten_bind_Coordinates_set_destination_2(self, lat, lng);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
Coordinates.prototype['__destroy__'] = Coordinates.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_Coordinates___destroy___0(self);
};
