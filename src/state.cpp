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
#include "state.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	State::State() : node::ObjectWrap() {
		_state = clutter_state_new();
	}

	void State::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("State");

		/* Methods */
		State::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void State::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		/* Inherit methods from Actor */
		Actor::PrototypeMethodsInit(constructor_template);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "set", State::Set);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setState", State::SetState);
	}

	/* ECMAScript constructor */
	Handle<Value> State::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		State* obj = new State();
		obj->Wrap(args.This());

		/* Set duration */
		ClutterState *instance = obj->_state;
		if (args[0]->IsNumber()) {
			clutter_state_set_duration(CLUTTER_STATE(instance), NULL, NULL, args[0]->ToInteger()->Value());
		}

		return args.This();
	}

	Handle<Value> State::Set(const Arguments &args)
	{
		HandleScope scope;
		Local<Value> SourceStateName;
		Local<Value> TargetStateName;
		Local<Value> Settings;
		ClutterAnimator *animator;
		GValue gvalue = {0};

		ClutterState *instance = ObjectWrap::Unwrap<State>(args.This())->_state;

		if (args.Length() == 2) {
			TargetStateName = args[0];
			Settings = args[1];
		} else if (args.Length() == 3) {
			SourceStateName = args[0];
			TargetStateName = args[1];
			Settings = args[2];
		} else {
			return args.This();
		}

		if (TargetStateName->IsString() && Settings->IsObject()) {
			Local<Array> settings = Local<Array>::Cast(Settings->ToObject());

			for (int i = 0; i < settings->Length(); ++i) {
				Local<Array> obj = Local<Array>::Cast(settings->Get(i)->ToObject());

				/* [ actor, property, alpha, value ] ] */
				ClutterActor *actor = ObjectWrap::Unwrap<Actor>(obj->Get(0)->ToObject())->_actor;
				Local<Value> PropertyName = obj->Get(1);
				Local<Value> Alpha = obj->Get(2);
				Local<Value> Value = obj->Get(3);

				if (!PropertyName->IsString() || !Value->IsNumber()) {
					continue;
				}

				/* Prepare Value */
				ObjectWrap::Unwrap<Actor>(obj->Get(0)->ToObject())->PropertyValueInit(&gvalue, PropertyName, Value);

				if (args.Length() == 2) {
					clutter_state_set_key(instance,
						NULL,
						*String::AsciiValue(TargetStateName->ToString()),
						G_OBJECT(actor),
						*String::AsciiValue(PropertyName->ToString()),
						Alpha->ToInteger()->Value(),
						&gvalue,
						0,
						0);
				} else {
					clutter_state_set(instance,
						*String::AsciiValue(SourceStateName->ToString()),
						*String::AsciiValue(TargetStateName->ToString()),
						G_OBJECT(actor),
						*String::AsciiValue(PropertyName->ToString()),
						Alpha->ToInteger()->Value(),
						&gvalue,
						0,
						0);
				}

				g_value_unset(&gvalue);
			}
		}

		return args.This();
	}

	Handle<Value> State::SetState(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			ClutterState *instance = ObjectWrap::Unwrap<State>(args.This())->_state;

			clutter_state_set_state(instance, *String::AsciiValue(args[0]->ToString()));
		}

		return args.This();
	}

}
