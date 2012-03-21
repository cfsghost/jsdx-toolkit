#ifndef JSDX_TOOLKIT_WIDGET_FRAME_H_
#define JSDX_TOOLKIT_WIDGET_FRAME_H_

#include <clutter/clutter.h>
#include <mx/mx.h>
#include <v8.h>
#include "widget.hpp"

namespace JSDXToolkit {

class Frame : public Widget {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Frame();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
};

}

#endif
