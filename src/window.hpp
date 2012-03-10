#ifndef NODE_CLUTTER_WINDOW_H_
#define NODE_CLUTTER_WINDOW_H_

#include <clutter/clutter.h>
#if ENABLE_MX
#include <mx/mx.h>
#endif
#include <v8.h>
#include "stage.hpp"

namespace clutter {

class Window : public Stage {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);
	static v8::Local<v8::Object> New(void);

#if ENABLE_MX
	MxWindow *_window;
#endif

protected:
	Window();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> HasToolbarGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void HasToolbarSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

}

#endif
