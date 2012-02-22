#ifndef NODE_CLUTTER_ACTOR_H_
#define NODE_CLUTTER_ACTOR_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class Actor : public node::ObjectWrap {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

	ClutterActor *_actor;

protected:
	Actor();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> Show(const v8::Arguments& args);
	static v8::Handle<v8::Value> ShowAll(const v8::Arguments& args);
	static v8::Handle<v8::Value> Hide(const v8::Arguments& args);

	static v8::Handle<v8::Value> SetOpacity(const v8::Arguments& args);
};

}

#endif
