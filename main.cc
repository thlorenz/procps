#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "procjs.h"

using namespace std;

v8::Handle<v8::String> GetScript(v8::Isolate* isolate);

static void AddFunction(v8::Isolate* isolate, v8::Handle<v8::Object> global, const char* name, v8::FunctionCallback callback) {
  using namespace v8;
  HandleScope handle_scope(isolate);
  global->Set(String::NewFromUtf8(isolate, name), FunctionTemplate::New(isolate, callback)->GetFunction());
}

int main(void) {
  using namespace v8;
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);
  Handle<Context> context = Context::New(isolate);
  Context::Scope context_scope(context);

  Handle<Object> global = context->Global();
  AddFunction(isolate, global, "Procjs", Procjs::New);

  Handle<String> src = ReadFile(isolate, "test.js");
  Handle<Value> result = Script::Compile(src)->Run();

  cout << "Result: " << endl << *String::Utf8Value(result);
  return 0;
}

// vim: ft=cpp
