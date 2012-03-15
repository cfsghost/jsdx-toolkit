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
#include "bin.hpp"
#include "scrollable.hpp"
#include "kinetic_scrollview.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> KineticScrollView::constructor;

	KineticScrollView::KineticScrollView() : Bin() {
		_actor = mx_kinetic_scroll_view_new();

		mx_kinetic_scroll_view_set_overshoot(MX_KINETIC_SCROLL_VIEW(_actor), 0.2);
	}

	void KineticScrollView::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		Local<String> name = String::NewSymbol("KineticScrollView");

		/* Constructor */
		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		/* Methods */
		Bin::PrototypeMethodsInit(constructor);

		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("policy_horizontal"), KineticScrollView::PolicyHorizontalGetter, KineticScrollView::PolicyHorizontalSetter);
		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("policy_vertical"), KineticScrollView::PolicyVerticalGetter, KineticScrollView::PolicyVerticalSetter);

		target->Set(name, constructor->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> KineticScrollView::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		KineticScrollView* obj = new KineticScrollView();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> KineticScrollView::PolicyHorizontalGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;
		MxScrollPolicy policy;

		ClutterActor *instance = ObjectWrap::Unwrap<KineticScrollView>(info.This())->_actor;

		policy = mx_kinetic_scroll_view_get_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance));
		if (policy == MX_SCROLL_POLICY_HORIZONTAL || policy == MX_SCROLL_POLICY_BOTH) {
			return scope.Close(Boolean::New(True));
		}

		return scope.Close(Boolean::New(False));
	}

	void KineticScrollView::PolicyHorizontalSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;
		MxScrollPolicy policy;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<KineticScrollView>(info.This())->_actor;

			policy = mx_kinetic_scroll_view_get_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance));

			if (value->ToBoolean()->Value()) {
				if (policy == MX_SCROLL_POLICY_VERTICAL)
					mx_kinetic_scroll_view_set_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance), MX_SCROLL_POLICY_BOTH);
				else if (policy == MX_SCROLL_POLICY_NONE)
					mx_kinetic_scroll_view_set_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance), MX_SCROLL_POLICY_HORIZONTAL);
			} else {
				if (policy == MX_SCROLL_POLICY_HORIZONTAL)
					mx_kinetic_scroll_view_set_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance), MX_SCROLL_POLICY_NONE);
				else if (policy == MX_SCROLL_POLICY_BOTH)
					mx_kinetic_scroll_view_set_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance), MX_SCROLL_POLICY_VERTICAL);
			}
		}
	}

	Handle<Value> KineticScrollView::PolicyVerticalGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;
		MxScrollPolicy policy;

		ClutterActor *instance = ObjectWrap::Unwrap<KineticScrollView>(info.This())->_actor;

		policy = mx_kinetic_scroll_view_get_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance));
		if (policy == MX_SCROLL_POLICY_VERTICAL || policy == MX_SCROLL_POLICY_BOTH) {
			return scope.Close(Boolean::New(True));
		}

		return scope.Close(Boolean::New(False));
	}

	void KineticScrollView::PolicyVerticalSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;
		MxScrollPolicy policy;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<KineticScrollView>(info.This())->_actor;

			policy = mx_kinetic_scroll_view_get_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance));
			if (value->ToBoolean()->Value()) {
				if (policy == MX_SCROLL_POLICY_HORIZONTAL)
					mx_kinetic_scroll_view_set_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance), MX_SCROLL_POLICY_BOTH);
				else if (policy == MX_SCROLL_POLICY_NONE)
					mx_kinetic_scroll_view_set_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance), MX_SCROLL_POLICY_VERTICAL);
			} else {
				if (policy == MX_SCROLL_POLICY_VERTICAL)
					mx_kinetic_scroll_view_set_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance), MX_SCROLL_POLICY_NONE);
				else if (policy == MX_SCROLL_POLICY_BOTH)
					mx_kinetic_scroll_view_set_scroll_policy(MX_KINETIC_SCROLL_VIEW(instance), MX_SCROLL_POLICY_HORIZONTAL);
			}
		}
	}

}
