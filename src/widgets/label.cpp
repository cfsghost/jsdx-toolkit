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
#include "label.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Label::Label() : Widget() {
		_actor = mx_label_new();
	}

	void Label::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Label");

		/* Methods */
		Label::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Label::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);
		Bin::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("text"), Label::TextGetter, Label::TextSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> Label::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Label* obj = new Label();
		obj->Wrap(args.This());

		if (args[0]->IsString()) {
			mx_label_set_text(MX_LABEL(obj->_actor), *String::Utf8Value(args[0]->ToString()));
		}

		return scope.Close(args.This());
	}

	Handle<Value> Label::TextGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Label>(info.This())->_actor;

		return scope.Close(
			String::New(mx_label_get_text(MX_LABEL(instance)))
		);
	}

	void Label::TextSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Label>(info.This())->_actor;

			mx_label_set_text(MX_LABEL(instance), *String::Utf8Value(value->ToString()));
		}
	}

}
