#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <purple.h>

using namespace v8;

Handle<Value> PurpleCoreSetUiOps(const Arguments& args) {
    HandleScope scope;
    Local<Value> directory = args[0];
    String::Utf8Value utf8directory(directory);
    
    purple_util_set_user_dir(*utf8directory);
    
    return scope.Close(Undefined());
}

Handle<Value> PurpleUtilSetUserDir(const Arguments& args) {
    HandleScope scope;
    Local<Value> directory = args[0];
    String::Utf8Value utf8directory(directory);

    purple_util_set_user_dir(*utf8directory);
    
    return scope.Close(Undefined());
}

Handle<Value> PurpleDebugSetEnabled(const Arguments& args) {
    HandleScope scope;
    bool flag = args[0]->BooleanValue();
    
    purple_debug_set_enabled(flag);
    
    return scope.Close(Undefined());
}

void init(Handle<Object> target) {
    target->Set(String::NewSymbol("purple_util_set_user_dir"),
                FunctionTemplate::New(PurpleUtilSetUserDir)->GetFunction());
    target->Set(String::NewSymbol("purple_debug_set_enabled"),
                FunctionTemplate::New(PurpleDebugSetEnabled)->GetFunction());
}
NODE_MODULE(purple, init)