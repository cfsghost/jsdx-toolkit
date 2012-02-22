#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "container.hpp"
#include "stage.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

Stage::Stage() : Container() {
	HandleScope scope;

	/* Create Stage */
	_actor = clutter_stage_new();

	/* TODO: Binding destroy event */
}

void Stage::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Local<String> name = String::NewSymbol("Stage");

	/* Methods */
	Container::PrototypeMethodsInit(tpl);

	NODE_SET_PROTOTYPE_METHOD(tpl, "setTitle", Stage::SetTitle);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getTitle", Stage::GetTitle);
	NODE_SET_PROTOTYPE_METHOD(tpl, "useAlpha", Stage::SetUseAlpha);
	NODE_SET_PROTOTYPE_METHOD(tpl, "setColor", Stage::SetColor);

	target->Set(name, tpl->GetFunction());
}

/* ECMAScript constructor */
Handle<Value> Stage::New(const Arguments& args)
{
	HandleScope scope;

	if (!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of this object."))
		);
	}

	// Creates a new instance object of this type and wraps it.
	Stage* obj = new Stage();
	obj->Wrap(args.This());

	return scope.Close(args.This());
}

Handle<Value> Stage::SetTitle(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsString()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_stage_set_title(CLUTTER_STAGE(instance), *String::Utf8Value(args[0]->ToString()));
	}

	return args.This();
}

Handle<Value> Stage::GetTitle(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(String::New(clutter_stage_get_title(CLUTTER_STAGE(instance))));
}

Handle<Value> Stage::SetUseAlpha(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsBoolean()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_stage_set_use_alpha(CLUTTER_STAGE(instance), args[0]->ToBoolean()->Value());
	}

	return args.This();
}

Handle<Value> Stage::SetColor(const Arguments &args)
{
	HandleScope scope;
	static ClutterColor color;

	if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		color.red = args[0]->NumberValue();
		color.green = args[1]->NumberValue();
		color.blue = args[2]->NumberValue();
		color.alpha = args[3]->NumberValue();

		clutter_stage_set_color(CLUTTER_STAGE(instance), &color);
	}

	return args.This();
}

}
