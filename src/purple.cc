#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <purple.h>

using namespace v8;
static Persistent<Function> notifier;

static void notify(const unsigned argc, Local<Value> argv[]) {
    notifier->Call(Context::GetCurrent()->Global(), argc, argv);
}

static void callbackUiPrefsInit() {
    Local<Value> argv[] = {
        String::New("ui_prefs_init")
    };
    notify(1, argv);
}

static void callbackDebugUiInit() {
    Local<Value> argv[] = {
        String::New("debug_init")
    };
    notify(1, argv);
}

static void callbackUiInit() {
    Local<Value> argv[] = {
        String::New("ui_init")
    };
    notify(1, argv);
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

/**
 * The following eventloop functions are used in both pidgin and purple-text. If your
 * application uses glib mainloop, you can safely use this verbatim.
 */
#define PURPLE_GLIB_READ_COND  (G_IO_IN | G_IO_HUP | G_IO_ERR)
#define PURPLE_GLIB_WRITE_COND (G_IO_OUT | G_IO_HUP | G_IO_ERR | G_IO_NVAL)

typedef struct _PurpleGLibIOClosure {
	PurpleInputFunction function;
	guint result;
	gpointer data;
} PurpleGLibIOClosure;

typedef struct
{
	PurpleAccountRequestType type;
	PurpleAccount *account;
	void *ui_handle;
	char *user;
	gpointer userdata;
	PurpleAccountRequestAuthorizationCb auth_cb;
	PurpleAccountRequestAuthorizationCb deny_cb;
	guint ref;
} PurpleAccountRequestInfo;

static void purple_glib_io_destroy(gpointer data)
{
	g_free(data);
}

static gboolean purple_glib_io_invoke(GIOChannel *source, GIOCondition condition, gpointer data)
{
	PurpleGLibIOClosure* closure = (PurpleGLibIOClosure*)data;
	PurpleInputCondition purple_cond;

	if (condition & PURPLE_GLIB_READ_COND) {
		purple_cond = PURPLE_INPUT_READ;
    }
	//if (condition & PURPLE_GLIB_WRITE_COND)
    else {
		purple_cond = PURPLE_INPUT_WRITE;
    }

	closure->function(closure->data, g_io_channel_unix_get_fd(source),
                      purple_cond);
    
	return TRUE;
}

static guint glib_input_add(gint fd, PurpleInputCondition condition, PurpleInputFunction function,
                            gpointer data)
{
	PurpleGLibIOClosure *closure = g_new0(PurpleGLibIOClosure, 1);
	GIOChannel *channel;
	GIOCondition cond;
    
	closure->function = function;
	closure->data = data;
    
	if (condition & PURPLE_INPUT_READ) {
		cond = G_IO_IN;
    }
	//if (condition & PURPLE_INPUT_WRITE)
    else {
		cond = G_IO_OUT;
    }

	channel = g_io_channel_unix_new(fd);
	closure->result = g_io_add_watch_full(channel, G_PRIORITY_DEFAULT, cond,
                                          purple_glib_io_invoke, closure, purple_glib_io_destroy);
    
	g_io_channel_unref(channel);
	return closure->result;
}

static PurpleEventLoopUiOps glib_eventloops =
{
	g_timeout_add,
	g_source_remove,
	glib_input_add,
	g_source_remove,
	NULL,
#if GLIB_CHECK_VERSION(2,14,0)
	g_timeout_add_seconds,
#else
	NULL,
#endif
    
	/* padding */
	NULL,
	NULL,
	NULL
};
/*** End of the eventloop functions. ***/

static Handle<Value> Initialize(const Arguments& args) {
    HandleScope scope;
    Handle<Function> callback = Local<Function>::Cast(args[0]);
    
    notifier = *callback;
        
	/* We do not want any debugging for now to keep the noise to a minimum. */
	purple_debug_set_enabled(TRUE);
    
	/* Set the core-uiops, which is used to
	 * 	- initialize the ui specific preferences.
	 * 	- initialize the debug ui.
	 * 	- initialize the ui components for all the modules.
	 * 	- uninitialize the ui components for all the modules when the core terminates.
	 */
	purple_core_set_ui_ops(&coreUiOps);
    
	/* Set the uiops for the eventloop. If your client is glib-based, you can safely
	 * copy this verbatim. */
	purple_eventloop_set_ui_ops(&glib_eventloops);
    
    return scope.Close(Undefined());
}

static const char* toCString(const Local<Value>& val) {
    String::Utf8Value utf8(val);
    
    return *utf8;
}

static Handle<Value> Start(const Arguments& args) {
    HandleScope scope;

	/* Set a custom user directory (optional) */
    purple_util_set_user_dir(toCString(args[0]));

	/* Set path to search for plugins. The core (libpurple) takes care of loading the
	 * core-plugins, which includes the protocol-plugins. So it is not essential to add
	 * any path here, but it might be desired, especially for ui-specific plugins. */
	purple_plugins_add_search_path(toCString(args[1]));
    
	/* Now that all the essential stuff has been set, let's try to init the core. It's
	 * necessary to provide a non-NULL name for the current ui to the core. This name
	 * is used by stuff that depends on this ui, for example the ui-specific plugins. */
	if (!purple_core_init("node-purple")) {
		/* Initializing the core failed. Terminate. */
        ThrowException(Exception::TypeError(String::New("libpurple initialization failed.")));
        return scope.Close(Undefined());
	}

	/* Create and load the buddylist. */
	purple_set_blist(purple_blist_new());
	purple_blist_load();
    
	/* Load the preferences. */
	purple_prefs_load();
    
	/* Load the desired plugins. The client should save the list of loaded plugins in
	 * the preferences using purple_plugins_save_loaded(PLUGIN_SAVE_PREF) */
	purple_plugins_load_saved(toCString(args[3]));
    
	/* Load the pounces. */
	purple_pounces_load();

    return scope.Close(Undefined());
}

void init(Handle<Object> target) {
    target->Set(String::NewSymbol("start"),
                FunctionTemplate::New(Start)->GetFunction());
    target->Set(String::NewSymbol("init"),
                FunctionTemplate::New(Initialize)->GetFunction());
}

NODE_MODULE(purple, init)
