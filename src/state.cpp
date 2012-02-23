#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "state.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

State::State() : node::ObjectWrap() {
	_state = clutter_state_new();
}

void State::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Local<String> name = String::NewSymbol("State");

	/* Methods */
	State::PrototypeMethodsInit(tpl);

	target->Set(name, tpl->GetFunction());
}

void State::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
{
	HandleScope scope;

	/* Inherit methods from Actor */
	Actor::PrototypeMethodsInit(constructor_template);

	NODE_SET_PROTOTYPE_METHOD(constructor_template, "set", State::Set);
}

/* ECMAScript constructor */
Handle<Value> State::New(const Arguments& args)
{
	HandleScope scope;

	if (!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of this object."))
		);
	}

	// Creates a new instance object of this type and wraps it.
	State* obj = new State();
	obj->Wrap(args.This());

	/* Set duration */
	ClutterState *instance = obj->_state;
	if (args[0]->IsNumber()) {
		clutter_state_set_duration(CLUTTER_STATE(instance), NULL, NULL, args[0]->ToInteger()->Value());
	} else {
		clutter_state_set_duration(CLUTTER_STATE(instance), NULL, NULL, 1000);
	}

	return args.This();
}

Handle<Value> State::Set(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
//	ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

//	clutter_container_add_actor(CLUTTER_CONTAINER(instance), CLUTTER_ACTOR(actor));

	return args.This();
}

}
