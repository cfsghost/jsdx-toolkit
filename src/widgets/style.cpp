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
#include "widget.hpp"
#include "bin.hpp"
#include "style.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Style::Style() : Widget() {
		_style = mx_style_new();
	}

	void Style::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Style");

		/* Methods */
		Style::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Style::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		/* Methods */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Style::LoadFile);
	}

	/* ECMAScript constructor */
	Handle<Value> Style::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Style* obj = new Style();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	/* Methods */
	Handle<Value> Style::LoadFile(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			MxStyle *instance = ObjectWrap::Unwrap<Style>(args.This())->_style;

			mx_style_load_from_file(MX_STYLE(instance), *String::Utf8Value(args[0]->ToString()), NULL);
		}

		return args.This();
	}

}
