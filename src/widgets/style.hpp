#ifndef NODE_CLUTTER_WIDGET_STYLE_H_
#define NODE_CLUTTER_WIDGET_STYLE_H_

#include <clutter/clutter.h>
#include <mx/mx.h>
#include <v8.h>
#include "widget.hpp"

namespace JSDXToolkit {

class Style : public Widget {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

	MxStyle *_style;

protected:
	Style();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> LoadFile(const v8::Arguments& args);
};

}

#endif
