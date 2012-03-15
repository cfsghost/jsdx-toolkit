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
#include "spinner.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Spinner::Spinner() : Widget() {
		_actor = mx_spinner_new();
	}

	void Spinner::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Spinner");

		/* Methods */
		Spinner::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Spinner::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("animating"), Spinner::AnimatingGetter, Spinner::AnimatingSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> Spinner::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Spinner* obj = new Spinner();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> Spinner::AnimatingGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Spinner>(info.This())->_actor;

		return scope.Close(
			Boolean::New(mx_spinner_get_animating(MX_SPINNER(instance)))
		);
	}

	void Spinner::AnimatingSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Spinner>(info.This())->_actor;

			mx_spinner_set_animating(MX_SPINNER(instance), value->ToBoolean()->Value());
		}
	}

}
