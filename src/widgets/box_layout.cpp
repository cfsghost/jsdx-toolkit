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
#include "../container.hpp"
#include "widget.hpp"
#include "bin.hpp"
#include "scrollable.hpp"
#include "box_layout.hpp"
#include "stylable.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> BoxLayout::constructor;

	BoxLayout::BoxLayout() : Widget() {
		_actor = mx_box_layout_new();
		mx_box_layout_set_orientation(MX_BOX_LAYOUT(_actor), (MxOrientation)JSDX_TOOLKIT_WIDGET_ORIENTATION_VERTICAL);
	}

	void BoxLayout::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		Local<String> name = String::NewSymbol("BoxLayout");

		/* Constructor */
		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		/* Methods */
		BoxLayout::PrototypeMethodsInit(constructor);

		target->Set(name, constructor->GetFunction());
	}

	void BoxLayout::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		/* Inherit methods from Actor */
		Container::PrototypeMethodsInit(constructor_template);
		Widget::PrototypeMethodsInit(constructor_template);
		Stylable::PrototypeMethodsInit(constructor_template);

		/* Accessor */
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("orientation"), BoxLayout::OrientationGetter, BoxLayout::OrientationSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("spacing"), BoxLayout::SpacingGetter, BoxLayout::SpacingSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("enableAnimations"), BoxLayout::EnableAnimationsGetter, BoxLayout::EnableAnimationsSetter);

		/* Methods */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_add", BoxLayout::Add);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_setExpand", BoxLayout::SetExpand);
	}

	/* ECMAScript constructor */
	Handle<Value> BoxLayout::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		BoxLayout* obj = new BoxLayout();
		obj->Wrap(args.This());

		/* Initializing sub-objects */
		Local<Object> ScrollableObject = Scrollable::New(MX_SCROLLABLE(obj->_actor));
		args.Holder()->Set(String::NewSymbol("scroll"), ScrollableObject);

		return scope.Close(args.This());
	}

	/* Accessor */
	Handle<Value> BoxLayout::OrientationGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<BoxLayout>(info.This())->_actor;

		return scope.Close(
			Integer::New(mx_box_layout_get_orientation(MX_BOX_LAYOUT(instance)))
		);
	}

	void BoxLayout::OrientationSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<BoxLayout>(info.This())->_actor;

			mx_box_layout_set_orientation(MX_BOX_LAYOUT(instance), (MxOrientation)value->ToInteger()->Value());
		}
	}

	Handle<Value> BoxLayout::SpacingGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<BoxLayout>(info.This())->_actor;

		return scope.Close(
			Integer::New(mx_box_layout_get_spacing(MX_BOX_LAYOUT(instance)))
		);
	}

	void BoxLayout::SpacingSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<BoxLayout>(info.This())->_actor;

			mx_box_layout_set_spacing(MX_BOX_LAYOUT(instance), value->ToInteger()->Value());
		}
	}

	Handle<Value> BoxLayout::EnableAnimationsGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<BoxLayout>(info.This())->_actor;

		return scope.Close(
			Boolean::New(mx_box_layout_get_enable_animations(MX_BOX_LAYOUT(instance)))
		);
	}

	void BoxLayout::EnableAnimationsSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<BoxLayout>(info.This())->_actor;

			mx_box_layout_set_enable_animations(MX_BOX_LAYOUT(instance), value->ToBoolean()->Value());
		}
	}

	/* Methods */
	Handle<Value> BoxLayout::Add(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsObject()) {
			gint index = -1;
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
			ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

			if (args[1]->IsNumber())
				index = args[1]->ToInteger()->Value();

			mx_box_layout_add_actor(MX_BOX_LAYOUT(instance), CLUTTER_ACTOR(actor), index);
		}

		return args.This();
	}

	Handle<Value> BoxLayout::SetExpand(const Arguments &args)
	{
		HandleScope scope;

		if (args.Length() == 2) {
			if (args[0]->IsObject() && args[1]->IsBoolean()) {
				ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
				ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

				if (clutter_actor_contains(instance, actor)) {
					mx_box_layout_child_set_expand(MX_BOX_LAYOUT(instance), CLUTTER_ACTOR(actor), args[1]->ToBoolean()->Value());
				}
			}
		}

		return args.This();
	}

}
