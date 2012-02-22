#ifndef NODE_CLUTTER_ACTOR_H_
#define NODE_CLUTTER_ACTOR_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class Actor : public node::ObjectWrap {
public:
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	Actor();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	ClutterActor *actor_;
};

}

#endif
