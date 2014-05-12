#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "procjs.h"

using namespace std;

v8::Handle<v8::String> GetScript(v8::Isolate* isolate);

int main(void) {
  using namespace v8;
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope handle_scope;
  Handle<Context> context = Context::New();
  Context::Scope context_scope(context);

  Handle<Object> global = context->Global();

  global->Set(v8::String::New("Procjs"), v8::FunctionTemplate::New(Procjs::New)->GetFunction());

  Handle<String> src = ReadFile(isolate, "test.js");
  Handle<Value> result = Script::Compile(src)->Run();

  cout << "Result: " << endl << *String::Utf8Value(result);
  return 0;
}

// vim: ft=cpp
