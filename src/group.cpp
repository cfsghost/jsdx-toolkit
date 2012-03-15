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
#include "group.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Group::Group() : Container() {
		HandleScope scope;

		/* Create Group */
		_actor = clutter_group_new();

		/* TODO: Binding destroy event */
	}

	void Group::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Group");

		/* Methods */
		Container::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> Group::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Group* obj = new Group();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

}
