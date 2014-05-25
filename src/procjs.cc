#include <node.h>
#include "procjs.h"
#include <cassert>

#define MAX_PROCS 5000

typedef v8::Handle<v8::Value> (*FunctionCallback)(const v8::Arguments& info);
void AddFunction(v8::Handle<v8::Object> global, const char* name, FunctionCallback callback) {
  global->Set(v8::String::New(name), v8::FunctionTemplate::New(callback)->GetFunction());
}

/*
  * By default readproc will consider all processes as valid to parse
  * and return, but not actually fill in the cmdline, environ, and /proc/#/statm
  * derived memory fields.
  *
  * `flags' (a bitwise-or of PROC_* below) modifies the default behavior.
  *
  * The "fill" options will cause more of the proc_t to be filled in.
  *
  * The "filter" options all use the second argument as the pointer to a list of objects:
  *    process status', process id's, user id's.
  *
  * The third argument is the length of the list (currently only used for lists of user
  * id's since uid_t supports no convenient termination sentinel.)
  */
static proc_t **get_proctab(uint32_t flags) {
  return readproctab(flags);
}

static v8::Handle<v8::Value> Readproctab(const v8::Arguments& args) {
  using namespace v8;
  HandleScope handle_scope;

  // our JS API ensures we always pass flags (defaults if none were given)
  Local<Integer> flags = args[0].As<Integer>();
  assert(flags->IsUint32());

  Local<Function> cb = args[1].As<Function>();
  assert(cb->IsFunction());

  proc_t **proctab = get_proctab(flags->Value());
  int len = -1;
  while(*(proctab + (++len)));

  assert(len <= MAX_PROCS && "exceeded MAX_PROCS");

  Local<Value> argv[MAX_PROCS];
  for (int i = 0; i < len; i++) {
    Proc *proc = new Proc(args.GetIsolate(), proctab[i]);
    argv[i] = proc->Wrap();
  }

  // Passing result by calling function with arguments instead of populating
  // and returning v8::Array since it's so much faster
  // This technique came from node: https://github.com/joyent/node/blob/5344d0c1034b28f9e6de914430d8c8436ad85105/src/node_file.cc#L326
  // (thanks @trevnorris for explaining it to me)
  cb->NewInstance(len, argv);

  return v8::Undefined();
}

void init(v8::Handle<v8::Object> exports) {
  exports->Set(v8::String::NewSymbol("readproctab"), v8::FunctionTemplate::New(Readproctab)->GetFunction());
}

NODE_MODULE(procjs, init)
