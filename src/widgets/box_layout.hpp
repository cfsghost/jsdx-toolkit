#ifndef JSDX_TOOLKIT_WIDGET_BOX_LAYOUT_H_
#define JSDX_TOOLKIT_WIDGET_BOX_LAYOUT_H_

#include <clutter/clutter.h>
#include <mx/mx.h>
#include <v8.h>
#include "widget.hpp"

namespace JSDXToolkit {

class BoxLayout : public Widget {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	BoxLayout();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	/* Accessor */
	static v8::Handle<v8::Value> OrientationGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void OrientationSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> SpacingGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void SpacingSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> EnableAnimationsGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void EnableAnimationsSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	/* Methods */
	static v8::Handle<v8::Value> Add(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetExpand(const v8::Arguments& args);
};

}

#endif
