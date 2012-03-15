#ifndef NODE_CLUTTER_WIDGETS_KINETIC_SCROLLVIEW_H_
#define NODE_CLUTTER_WIDGETS_KINETIC_SCROLLVIEW_H_

#include <clutter/clutter.h>
#include <v8.h>

namespace JSDXToolkit {

class KineticScrollView : public Bin {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	KineticScrollView();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> PolicyHorizontalGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void PolicyHorizontalSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> PolicyVerticalGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void PolicyVerticalSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

}

#endif
