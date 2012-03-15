#ifndef NODE_CLUTTER_WIDGET_H_
#define NODE_CLUTTER_WIDGET_H_

#include <clutter/clutter.h>
#include <v8.h>

namespace JSDXToolkit {

class Widget : public Actor {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Widget();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> DisabledGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void DisabledSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

}

#endif
