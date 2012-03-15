#ifndef JDDX_TOOLKIT_WIDGET_DIALOG_H_
#define JDDX_TOOLKIT_WIDGET_DIALOG_H_

#include <clutter/clutter.h>
#include <mx/mx.h>
#include <v8.h>
#include "widget.hpp"

namespace JSDXToolkit {

class Dialog : public Widget {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Dialog();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetTransientParent(const v8::Arguments& args);
};

}

#endif
