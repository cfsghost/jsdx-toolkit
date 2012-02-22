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

	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setOpacity", Actor::SetOpacity);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "resize", Actor::Resize);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setWidth", Actor::SetWidth);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setHeight", Actor::SetHeight);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setPosition", Actor::SetPosition);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setDepth", Actor::SetDepth);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getDepth", Actor::GetDepth);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setX", Actor::SetX);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getX", Actor::GetX);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setY", Actor::SetY);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getY", Actor::GetY);
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

Handle<Value> Actor::SetOpacity(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_opacity(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::Resize(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber() && args[1]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_size(CLUTTER_ACTOR(instance), args[0]->NumberValue(), args[1]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::SetWidth(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_width(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::SetHeight(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_height(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::SetPosition(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber() && args[1]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_position(CLUTTER_ACTOR(instance), args[0]->NumberValue(), args[1]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::SetX(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_x(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetX(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Number::New(clutter_actor_get_x(CLUTTER_ACTOR(instance)))
	);
}

Handle<Value> Actor::SetY(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_y(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetY(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Number::New(clutter_actor_get_y(CLUTTER_ACTOR(instance)))
	);
}

Handle<Value> Actor::SetDepth(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_depth(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetDepth(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Number::New(clutter_actor_get_depth(CLUTTER_ACTOR(instance)))
	);
}

}
