#ifndef JSDX_TOOLKIT_WIDGETS_KINETIC_SCROLLVIEW_H_
#define JSDX_TOOLKIT_WIDGETS_KINETIC_SCROLLVIEW_H_

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

	static v8::Handle<v8::Value> StateGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void StateSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> ClampModeGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void ClampModeSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> ClampToCenterGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void ClampToCenterSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> OvershootGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void OvershootSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

}

#endif
