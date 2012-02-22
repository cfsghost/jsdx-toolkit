#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "stage.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

Stage::Stage() : Actor() {
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
	Actor::PrototypeMethodsInit(tpl);
	NODE_SET_PROTOTYPE_METHOD(tpl, "setTitle", Stage::SetTitle);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getTitle", Stage::GetTitle);
	NODE_SET_PROTOTYPE_METHOD(tpl, "useAlpha", Stage::SetUseAlpha);

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

}
