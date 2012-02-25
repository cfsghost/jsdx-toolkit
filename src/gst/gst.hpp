#ifndef NODE_CLUTTER_GST_H_
#define NODE_CLUTTER_GST_H_

#include <v8.h>
#include <node.h>

#include "../actor.hpp"
#include "../texture.hpp"
#include "../media.hpp"

namespace clutter {

class Gst : public Texture {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Gst();

	static v8::Handle<v8::Value> Init(const v8::Arguments& args);
};

}

#endif
