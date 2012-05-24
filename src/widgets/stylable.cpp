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

#include "../jsdx_toolkit.hpp"
#include "stylable.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> Stylable::constructor;

	Stylable::Stylable(MxStylable *scrollable) : ObjectWrap() {
		_scrollable = scrollable;
	}

	void Stylable::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		/* Template */
		Local<FunctionTemplate> tpl = FunctionTemplate::New();
		Local<String> name = String::NewSymbol("Stylable");

		/* Constructor */
		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		target->Set(name, constructor->GetFunction());
	}

	void Stylable::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("style");

		/* Stylable Object */
		Handle<ObjectTemplate> ObjectTpl = ObjectTemplate::New();
		ObjectTpl->SetInternalFieldCount(1);
		Local<Object> ObjectInstance = ObjectTpl->NewInstance();

		constructor_template->InstanceTemplate()->Set(name, ObjectInstance);
	}

	Local<Object> Stylable::New(MxStylable *stylable)
	{
		HandleScope scope;
		Local<Object> AdjustmentObject;

		/* Stylable Object */
		Local<Object> ObjectInstance = Stylable::constructor->GetFunction()->NewInstance();
		Stylable *obj = new Stylable(stylable);
		obj->Wrap(ObjectInstance);

		return ObjectInstance;
	}

}
