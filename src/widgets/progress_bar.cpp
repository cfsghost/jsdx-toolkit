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
#include "progress_bar.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	ProgressBar::ProgressBar() : Widget() {
		_actor = mx_progress_bar_new();
	}

	void ProgressBar::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("ProgressBar");

		/* Methods */
		ProgressBar::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void ProgressBar::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);
		Bin::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("progress"), ProgressBar::ProgressGetter, ProgressBar::ProgressSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> ProgressBar::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		ProgressBar* obj = new ProgressBar();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> ProgressBar::ProgressGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<ProgressBar>(info.This())->_actor;

		return scope.Close(
			Number::New(mx_progress_bar_get_progress(MX_PROGRESS_BAR(instance)))
		);
	}

	void ProgressBar::ProgressSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<ProgressBar>(info.This())->_actor;

			mx_progress_bar_set_progress(MX_PROGRESS_BAR(instance), value->NumberValue());
		}
	}

}
