#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "rectangle.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

Rectangle::Rectangle() : Actor() {
	HandleScope scope;

	/* Create Rectangle */
	_actor = clutter_rectangle_new();

	/* TODO: Binding destroy event */
}

void Rectangle::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Local<String> name = String::NewSymbol("Rectangle");

	/* Methods */
	Actor::PrototypeMethodsInit(tpl);

	target->Set(name, tpl->GetFunction());
}

/* ECMAScript constructor */
Handle<Value> Rectangle::New(const Arguments& args)
{
	HandleScope scope;
	static ClutterColor color;

	if (!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of this object."))
		);
	}

	// Creates a new instance object of this type and wraps it.
	Rectangle* obj = new Rectangle();
	obj->Wrap(args.This());

	/* Set color */
	if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
		ClutterActor *instance = obj->_actor;

		color.red = args[0]->NumberValue();
		color.green = args[1]->NumberValue();
		color.blue = args[2]->NumberValue();
		color.alpha = args[3]->NumberValue();

		clutter_rectangle_set_color(CLUTTER_RECTANGLE(instance), &color);
	}

	return scope.Close(args.This());
}

}
