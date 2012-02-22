#ifndef NODE_CLUTTER_STAGE_H_
#define NODE_CLUTTER_STAGE_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class Stage : public Actor {
public:
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	Stage();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
};

}

#endif
