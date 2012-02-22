#ifndef NODE_CLUTTER_RECTANGLE_H_
#define NODE_CLUTTER_RECTANGLE_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class Rectangle : public Actor {
public:
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	Rectangle();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetColor(const v8::Arguments& args);
};

}

#endif
