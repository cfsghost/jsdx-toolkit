#ifndef NODE_CLUTTER_APPLICATION_H_
#define NODE_CLUTTER_APPLICATION_H_

#include <clutter/clutter.h>
#if ENABLE_MX
#include <mx/mx.h>
#endif
#include <v8.h>

namespace clutter {

class Application : public node::ObjectWrap {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

#if ENABLE_MX
	MxApplication *_application;
#endif

protected:
	Application(const char *name);

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> CreateWindow(const v8::Arguments &args);
};

}

#endif