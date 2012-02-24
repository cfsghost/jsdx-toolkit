#ifndef NODE_CLUTTER_STATE_H_
#define NODE_CLUTTER_STATE_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class State : public node::ObjectWrap {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

	ClutterState *_state;

protected:
	State();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> Set(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetState(const v8::Arguments& args);
};

}

#endif
