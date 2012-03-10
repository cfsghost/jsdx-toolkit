#ifndef NODE_CLUTTER_WIDGET_SCROLLABLE_H_
#define NODE_CLUTTER_WIDGET_SCROLLABLE_H_

#include <v8.h>
#include <node.h>
#include "adjustment.hpp"

namespace clutter {

class Scrollable : public node::ObjectWrap {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);
	static v8::Local<v8::Object> New(MxScrollable *scrollable);

	MxScrollable *_scrollable;

	MxAdjustment *hadjust;
	MxAdjustment *vadjust;
	Adjustment *_hadjust;
	Adjustment *_vadjust;

protected:

	Scrollable(MxScrollable *scrollable);

	static v8::Handle<v8::Value> Test(const v8::Arguments& args);
};

}

#endif
