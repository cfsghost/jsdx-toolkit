#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "text.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

Text::Text() : Actor() {
	HandleScope scope;

	/* Create Text */
	_actor = clutter_text_new();
}

void Text::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Local<String> name = String::NewSymbol("Text");

	/* Methods */
	Actor::PrototypeMethodsInit(tpl);
	NODE_SET_PROTOTYPE_METHOD(tpl, "setColor", Text::SetColor);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getColor", Text::GetColor);

	target->Set(name, tpl->GetFunction());
}

/* ECMAScript constructor */
Handle<Value> Text::New(const Arguments& args)
{
	HandleScope scope;
	static ClutterColor color;

	if (!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of this object."))
		);
	}

	// Creates a new instance object of this type and wraps it.
	Text* obj = new Text();
	obj->Wrap(args.This());

	/* Set color */
	if (args.Length() > 0) {
		ClutterActor *instance = obj->_actor;

		if (args[0]->IsString())
			clutter_text_set_text(CLUTTER_TEXT(instance), *String::Utf8Value(args[0]->ToString()));
	}

	return scope.Close(args.This());
}

Handle<Value> Text::SetColor(const Arguments &args)
{
	HandleScope scope;
	static ClutterColor color;

	if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		color.red = args[0]->NumberValue();
		color.green = args[1]->NumberValue();
		color.blue = args[2]->NumberValue();
		color.alpha = args[3]->NumberValue();

		clutter_text_set_color(CLUTTER_TEXT(instance), &color);
	}

	return args.This();
}

Handle<Value> Text::GetColor(const Arguments &args)
{
	HandleScope scope;
	Local<Object> o;
	static ClutterColor color;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	clutter_text_get_color(CLUTTER_TEXT(instance), &color);

	/* create a JavaScript Object */
	o = Object::New();
	o->Set(String::New("r"), Integer::New(color.red));
	o->Set(String::New("g"), Integer::New(color.green));
	o->Set(String::New("b"), Integer::New(color.blue));
	o->Set(String::New("a"), Integer::New(color.alpha));

	return scope.Close(o);
}

}
