#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <purple.h>

using namespace v8;

const unsigned short CORE_UI_OPS_COUNT=3;
static Persistent<Function> coreUICallbacks[CORE_UI_OPS_COUNT];

static void runCoreCallback(unsigned short idx) {
    Handle<Function> cb = coreUICallbacks[idx];
    Local<Value> argv[0];
    
    if (!cb.IsEmpty()) {
        cb->Call(Context::GetCurrent()->Global(), 0, argv);
    }
}

static void callbackUiPrefsInit() {
    runCoreCallback(0);
}

static void callbackDebugUiInit() {
    runCoreCallback(1);
}

static void callbackUiInit() {
    runCoreCallback(2);
}

static PurpleCoreUiOps coreUiOps = {
    callbackUiPrefsInit,
    callbackDebugUiInit,
    callbackUiInit,
    NULL, // TODO: We should return some version number
    NULL,
    NULL,
    NULL,
    NULL
};

static Handle<Value> PurpleCoreSetUiOps(const Arguments& args) {
    HandleScope scope;
    
    for(unsigned idx=0; idx<CORE_UI_OPS_COUNT; idx++) {
        Handle<Function> callback = Local<Function>::Cast(args[0]);

        coreUICallbacks[idx] = *callback;
    }
    
    purple_core_set_ui_ops(&coreUiOps);
    
    return scope.Close(Undefined());
}

static Handle<Value> PurpleUtilSetUserDir(const Arguments& args) {
    HandleScope scope;
    Local<Value> directory = args[0];
    String::Utf8Value utf8directory(directory);

    purple_util_set_user_dir(*utf8directory);
    
    return scope.Close(Undefined());
}

static Handle<Value> PurpleDebugSetEnabled(const Arguments& args) {
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
    target->Set(String::NewSymbol("purple_core_set_ui_ops"),
                FunctionTemplate::New(PurpleCoreSetUiOps)->GetFunction());
}
NODE_MODULE(purple, init)