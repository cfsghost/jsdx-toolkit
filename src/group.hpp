#ifndef NODE_CLUTTER_GROUP_H_
#define NODE_CLUTTER_GROUP_H_

#include <v8.h>
#include <node.h>

namespace clutter {

class Group : public Container {
public:
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	Group();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
};

}

#endif
