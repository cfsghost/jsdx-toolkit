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

#include "../clutter.hpp"
#include "adjustment.hpp"
#include "scrollable.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> Scrollable::constructor;

	Scrollable::Scrollable(MxScrollable *scrollable) : ObjectWrap() {
		mx_scrollable_get_adjustments(MX_SCROLLABLE(scrollable), &hadjust, &vadjust);
	}

	void Scrollable::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		/* Template */
		Local<FunctionTemplate> tpl = FunctionTemplate::New();
		Local<String> name = String::NewSymbol("Scroll");

		/* Constructor */
		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		NODE_SET_PROTOTYPE_METHOD(constructor, "test", Scrollable::Test);

		target->Set(name, constructor->GetFunction());
	}

	void Scrollable::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("scroll");

		/* Scrollable Object */
		Handle<ObjectTemplate> ObjectTpl = ObjectTemplate::New();
		ObjectTpl->SetInternalFieldCount(1);
		Local<Object> ObjectInstance = ObjectTpl->NewInstance();

		/* Methods */
		NODE_SET_METHOD(ObjectInstance, "test", Scrollable::Test);

		constructor_template->InstanceTemplate()->Set(name, ObjectInstance);
	}

	Local<Object> Scrollable::New(MxScrollable *scrollable)
	{
		HandleScope scope;
		Local<Object> AdjustmentObject;

		/* Scrollable Object */
		Local<Object> ObjectInstance = Scrollable::constructor->GetFunction()->NewInstance();
		Scrollable *obj = new Scrollable(scrollable);
		obj->Wrap(ObjectInstance);

		/* Initializing sub-objects */
		AdjustmentObject = Adjustment::New(MX_ADJUSTMENT(obj->hadjust));
		ObjectInstance->Set(String::NewSymbol("horizontal"), AdjustmentObject);

		AdjustmentObject = Adjustment::New(MX_ADJUSTMENT(obj->vadjust));
		ObjectInstance->Set(String::NewSymbol("vertical"), AdjustmentObject);

		return ObjectInstance;
	}

	Handle<Value> Scrollable::Test(const Arguments& args)
	{
		HandleScope scope;

		return scope.Close(Number::New(123123));
	}

}
