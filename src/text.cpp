/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "jsdx_toolkit.hpp"
#include "actor.hpp"
#include "text.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Text::Text() : Actor() {
		HandleScope scope;

		/* Create Text */
		_actor = clutter_text_new();
	}

	void Text::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Text");

		PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Text::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		/* Inherit */
		Actor::PrototypeMethodsInit(constructor_template);

		/* Methods */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "focus", Text::Focus);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setColor", Text::SetColor);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "getColor", Text::GetColor);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setFontName", Text::SetFontName);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "getFontName", Text::GetFontName);

		/* Accessor */
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("text"), Text::TextGetter, Text::TextSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("editable"), Text::EditableGetter, Text::EditableSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> Text::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Text* obj = new Text();
		obj->Wrap(args.This());

		/* Set color */
		if (args.Length() > 0) {
			ClutterActor *instance = obj->_actor;

			if (args[0]->IsString())
				clutter_text_set_text(CLUTTER_TEXT(instance), *String::Utf8Value(args[0]->ToString()));
		}

		return scope.Close(args.This());
	}

	/* Accessor */
	Handle<Value> Text::TextGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			String::New(clutter_text_get_text(CLUTTER_TEXT(instance)))
		);
	}

	void Text::TextSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_text_set_text(CLUTTER_TEXT(instance), *String::Utf8Value(value->ToString()));
		}
	}

	Handle<Value> Text::EditableGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Boolean::New(clutter_text_get_editable(CLUTTER_TEXT(instance)))
		);
	}

	void Text::EditableSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_text_set_editable(CLUTTER_TEXT(instance), value->ToBoolean()->Value());
		}
	}

	/* Methods */
	Handle<Value> Text::Focus(const Arguments &args)
	{
		HandleScope scope;
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_stage_set_key_focus(CLUTTER_STAGE(clutter_actor_get_stage(actor)), actor);

		return args.This();
	}

	Handle<Value> Text::SetColor(const Arguments &args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			color.red = args[0]->NumberValue();
			color.green = args[1]->NumberValue();
			color.blue = args[2]->NumberValue();
			color.alpha = args[3]->NumberValue();

			clutter_text_set_color(CLUTTER_TEXT(instance), &color);
		}

		return args.This();
	}

	Handle<Value> Text::GetColor(const Arguments &args)
	{
		HandleScope scope;
		Local<Object> o;
		static ClutterColor color;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_text_get_color(CLUTTER_TEXT(instance), &color);

		/* create a JavaScript Object */
		o = Object::New();
		o->Set(String::New("r"), Integer::New(color.red));
		o->Set(String::New("g"), Integer::New(color.green));
		o->Set(String::New("b"), Integer::New(color.blue));
		o->Set(String::New("a"), Integer::New(color.alpha));

		return scope.Close(o);
	}

	Handle<Value> Text::SetFontName(const Arguments &args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (args[0]->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_text_set_font_name(CLUTTER_TEXT(instance), *String::Utf8Value(args[0]->ToString()));
		}

		return args.This();
	}

	Handle<Value> Text::GetFontName(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		return scope.Close(
			String::New(clutter_text_get_font_name(CLUTTER_TEXT(instance)))
		);
	}

}
