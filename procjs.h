#ifndef __PROCJS_H__
#define __PROCJS_H__

#include "v8.h"
v8::Handle<v8::String> ReadFile(v8::Isolate* isolate, const char* name);
#endif
