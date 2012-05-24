#ifndef JSDX_TOOLKIT_WIDGET_STYLABLE_H_
#define JSDX_TOOLKIT_WIDGET_STYLABLE_H_

#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

class Stylable : public node::ObjectWrap {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);
	static v8::Local<v8::Object> New(MxStylable *scrollable);

	MxStylable *_scrollable;

protected:

	Stylable(MxStylable *stylable);
};

}

#endif
