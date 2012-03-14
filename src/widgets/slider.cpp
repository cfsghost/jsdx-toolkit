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
#include "slider.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Slider::Slider() : Widget() {
		_actor = mx_slider_new();
	}

	void Slider::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Slider");

		/* Methods */
		Slider::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Slider::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("value"), Slider::ValueGetter, Slider::ValueSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> Slider::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Slider* obj = new Slider();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> Slider::ValueGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Slider>(info.This())->_actor;

		return scope.Close(
			Number::New(mx_slider_get_value(MX_SLIDER(instance)))
		);
	}

	void Slider::ValueSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Slider>(info.This())->_actor;

			mx_slider_set_value(MX_SLIDER(instance), value->NumberValue());
		}
	}

}
