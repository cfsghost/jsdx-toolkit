#ifndef NODE_CLUTTER_CONTAINER_H_
#define NODE_CLUTTER_CONTAINER_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

	class Container : public Actor {
	public:
		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

	protected:
		Container();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> Add(const v8::Arguments& args);
		static v8::Handle<v8::Value> Remove(const v8::Arguments& args);
	};

}

#endif
