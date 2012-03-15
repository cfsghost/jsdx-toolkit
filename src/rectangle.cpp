/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "rectangle.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Rectangle::Rectangle() : Actor() {
		HandleScope scope;

		/* Create Rectangle */
		_actor = clutter_rectangle_new();

		/* TODO: Binding destroy event */
	}

	void Rectangle::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Rectangle");

		/* Methods */
		Actor::PrototypeMethodsInit(tpl);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setColor", Rectangle::SetColor);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getColor", Rectangle::GetColor);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setBorderColor", Rectangle::SetBorderColor);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getBorderColor", Rectangle::GetBorderColor);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setBorder", Rectangle::SetBorderWidth);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getBorder", Rectangle::GetBorderWidth);

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> Rectangle::New(const Arguments& args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Rectangle* obj = new Rectangle();
		obj->Wrap(args.This());

		/* Set color */
		if (args.Length() > 0) {
			if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
				ClutterActor *instance = obj->_actor;

				color.red = args[0]->NumberValue();
				color.green = args[1]->NumberValue();
				color.blue = args[2]->NumberValue();
				color.alpha = args[3]->NumberValue();

				clutter_rectangle_set_color(CLUTTER_RECTANGLE(instance), &color);
			}
		}

		return scope.Close(args.This());
	}

	Handle<Value> Rectangle::SetColor(const Arguments &args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			color.red = args[0]->NumberValue();
			color.green = args[1]->NumberValue();
			color.blue = args[2]->NumberValue();
			color.alpha = args[3]->NumberValue();

			clutter_rectangle_set_color(CLUTTER_RECTANGLE(instance), &color);
		}

		return args.This();
	}

	Handle<Value> Rectangle::GetColor(const Arguments &args)
	{
		HandleScope scope;
		Local<Object> o;
		static ClutterColor color;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_rectangle_get_color(CLUTTER_RECTANGLE(instance), &color);

		/* create a JavaScript Object */
		o = Object::New();
		o->Set(String::New("r"), Integer::New(color.red));
		o->Set(String::New("g"), Integer::New(color.green));
		o->Set(String::New("b"), Integer::New(color.blue));
		o->Set(String::New("a"), Integer::New(color.alpha));

		return scope.Close(o);
	}

	Handle<Value> Rectangle::SetBorderColor(const Arguments &args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			color.red = args[0]->NumberValue();
			color.green = args[1]->NumberValue();
			color.blue = args[2]->NumberValue();
			color.alpha = args[3]->NumberValue();

			clutter_rectangle_set_border_color(CLUTTER_RECTANGLE(instance), &color);
		}

		return args.This();
	}

	Handle<Value> Rectangle::GetBorderColor(const Arguments &args)
	{
		HandleScope scope;
		Local<Object> o;
		static ClutterColor color;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_rectangle_get_border_color(CLUTTER_RECTANGLE(instance), &color);

		/* create a JavaScript Object */
		o = Object::New();
		o->Set(String::New("r"), Integer::New(color.red));
		o->Set(String::New("g"), Integer::New(color.green));
		o->Set(String::New("b"), Integer::New(color.blue));
		o->Set(String::New("a"), Integer::New(color.alpha));

		return scope.Close(o);
	}

	Handle<Value> Rectangle::SetBorderWidth(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_rectangle_set_border_width(CLUTTER_RECTANGLE(instance), args[0]->NumberValue());
		}

		return args.This();
	}

	Handle<Value> Rectangle::GetBorderWidth(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		return scope.Close(
			Integer::New(clutter_rectangle_get_border_width(CLUTTER_RECTANGLE(instance)))
		);
	}

}
