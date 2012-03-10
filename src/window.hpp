#ifndef NODE_CLUTTER_WINDOW_H_
#define NODE_CLUTTER_WINDOW_H_

#include <clutter/clutter.h>
#if ENABLE_MX
#include <mx/mx.h>
#endif
#include <v8.h>

namespace clutter {

class Window : public node::ObjectWrap {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);
	static v8::Local<v8::Object> New(void);

	v8::Local<v8::Object> StageObject;

#if ENABLE_MX
	MxWindow *_window;
#endif

protected:
	Window();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
};

}

#endif
