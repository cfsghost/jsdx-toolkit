#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

Actor::Actor()
	: ObjectWrap() {}

void Actor::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	Local<String> name = String::NewSymbol("Actor");

	PrototypeMethodsInit(tpl);

	target->Set(name, tpl->GetFunction());
}

void Actor::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
{
	HandleScope scope;

	NODE_SET_PROTOTYPE_METHOD(constructor_template, "show", Actor::Show);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "showAll", Actor::ShowAll);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "hide", Actor::Hide);
}

Handle<Value> Actor::New(const Arguments& args)
{
	HandleScope scope;

	if (!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of this object."))
		);
	}

	// Creates a new instance object of this type and wraps it.
	Actor* obj = new Actor();
	obj->Wrap(args.This());

	return args.This();
}

Handle<Value> Actor::Show(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
	clutter_actor_show(instance);

	return args.This();
}

Handle<Value> Actor::ShowAll(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
	clutter_actor_show_all(instance);

	return args.This();
}

Handle<Value> Actor::Hide(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
	clutter_actor_hide(instance);

	return args.This();
}

}
