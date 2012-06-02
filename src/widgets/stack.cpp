/*
 * JSDX Stackwork
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
#include "stack.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Stack::Stack() : Widget() {
		_actor = mx_stack_new();
	}

	void Stack::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Stack");

		/* Methods */
		Stack::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Stack::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Container::PrototypeMethodsInit(constructor_template);
	}

	/* ECMAScript constructor */
	Handle<Value> Stack::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Stack* obj = new Stack();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

}
