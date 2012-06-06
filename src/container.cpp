/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "jsdx_toolkit.hpp"
#include "actor.hpp"
#include "container.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Container::Container() : Actor() {}

	void Container::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Container");

		/* Methods */
		Container::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Container::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		/* Inherit methods from Actor */
		Actor::PrototypeMethodsInit(constructor_template);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_add", Container::Add);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_remove", Container::Remove);
	}

	/* ECMAScript constructor */
	Handle<Value> Container::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Container* obj = new Container();
		obj->Wrap(args.This());

		return args.This();
	}

	Handle<Value> Container::Add(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

		clutter_container_add_actor(CLUTTER_CONTAINER(instance), CLUTTER_ACTOR(actor));

		return args.This();
	}

	Handle<Value> Container::Remove(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;
		Actor *obj = ObjectWrap::Unwrap<Actor>(args[0]->ToObject());

		clutter_container_remove_actor(CLUTTER_CONTAINER(instance), CLUTTER_ACTOR(actor));

		obj->_actor = NULL;
		obj->_animation = NULL;

		return args.This();
	}

}
