#ifndef NODE_CLUTTER_WIDGET_SPINNER_H_
#define NODE_CLUTTER_WIDGET_SPINNER_H_

#include <clutter/clutter.h>
#include <mx/mx.h>
#include <v8.h>
#include "widget.hpp"

namespace clutter {

class Spinner : public Widget {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Spinner();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> AnimatingGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void AnimatingSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

}

#endif