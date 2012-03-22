#ifndef JSDX_TOOLKIT_WIDGET_BIN_H_
#define JSDX_TOOLKIT_WIDGET_BIN_H_

#include <clutter/clutter.h>
#include <v8.h>
#include "widget.hpp"

namespace JSDXToolkit {

class Bin : public Widget {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Bin();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> Add(const v8::Arguments& args);
};

}

#endif
