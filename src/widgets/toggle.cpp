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

#include "../clutter.hpp"
#include "../actor.hpp"
#include "widget.hpp"
#include "bin.hpp"
#include "toggle.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Toggle::Toggle() : Widget() {
		_actor = mx_toggle_new();
	}

	void Toggle::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Toggle");

		/* Methods */
		Toggle::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Toggle::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("active"), Toggle::ActiveGetter, Toggle::ActiveSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> Toggle::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Toggle* obj = new Toggle();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> Toggle::ActiveGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Toggle>(info.This())->_actor;

		return scope.Close(
			Boolean::New(mx_toggle_get_active(MX_TOGGLE(instance)))
		);
	}

	void Toggle::ActiveSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Toggle>(info.This())->_actor;

			mx_toggle_set_active(MX_TOGGLE(instance), value->ToBoolean()->Value());
		}
	}

}
