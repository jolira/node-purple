#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <purple.h>

using namespace v8;

Handle<Value> Method(const Arguments& args) {
    HandleScope scope;
    return scope.Close(String::New("world"));
}

void init(Handle<Object> target) {
    target->Set(String::NewSymbol("hello"),
                FunctionTemplate::New(Method)->GetFunction());
}
NODE_MODULE(purple, init)