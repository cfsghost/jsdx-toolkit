/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <mx/mx.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../jsdx_toolkit.hpp"
#include "../actor.hpp"
#include "../container.hpp"
#include "widget.hpp"
#include "bin.hpp"
#include "stylable.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Bin::Bin() : Widget() {}

	void Bin::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Bin");

		/* Methods */
		Bin::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Bin::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		/* Inherit methods from Actor */
		Container::PrototypeMethodsInit(constructor_template);
		Stylable::PrototypeMethodsInit(constructor_template);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_add", Bin::Add);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "focus", Bin::Focus);
	}

	/* ECMAScript constructor */
	Handle<Value> Bin::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Bin* obj = new Bin();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	/* Methods */
	Handle<Value> Bin::Add(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsObject()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Bin>(args.This())->_actor;
			ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

			mx_bin_set_child(MX_BIN(instance), CLUTTER_ACTOR(actor));
		}
	}

	Handle<Value> Bin::Focus(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Bin>(args.This())->_actor;
		ClutterActor *stage = clutter_actor_get_stage(instance);

		mx_focus_manager_push_focus(mx_focus_manager_get_for_stage(CLUTTER_STAGE(stage)), MX_FOCUSABLE(instance));
	}
}
