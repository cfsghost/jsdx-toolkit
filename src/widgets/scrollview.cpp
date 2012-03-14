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
#include "scrollview.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	ScrollView::ScrollView() : Bin() {
		_actor = mx_scroll_view_new();
	}

	void ScrollView::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("ScrollView");

		/* Methods */
		Bin::PrototypeMethodsInit(tpl);

		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("policy_horizontal"), ScrollView::PolicyHorizontalGetter, ScrollView::PolicyHorizontalSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("policy_vertical"), ScrollView::PolicyVerticalGetter, ScrollView::PolicyVerticalSetter);

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> ScrollView::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		ScrollView* obj = new ScrollView();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> ScrollView::PolicyHorizontalGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;
		MxScrollPolicy policy;

		ClutterActor *instance = ObjectWrap::Unwrap<ScrollView>(info.This())->_actor;

		policy = mx_scroll_view_get_scroll_policy(MX_SCROLL_VIEW(instance));
		if (policy == MX_SCROLL_POLICY_HORIZONTAL || policy == MX_SCROLL_POLICY_BOTH) {
			return scope.Close(Boolean::New(True));
		}

		return scope.Close(Boolean::New(False));
	}

	void ScrollView::PolicyHorizontalSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;
		MxScrollPolicy policy;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<ScrollView>(info.This())->_actor;

			policy = mx_scroll_view_get_scroll_policy(MX_SCROLL_VIEW(instance));

			if (value->ToBoolean()->Value()) {
				if (policy == MX_SCROLL_POLICY_VERTICAL)
					mx_scroll_view_set_scroll_policy(MX_SCROLL_VIEW(instance), MX_SCROLL_POLICY_BOTH);
				else if (policy == MX_SCROLL_POLICY_NONE)
					mx_scroll_view_set_scroll_policy(MX_SCROLL_VIEW(instance), MX_SCROLL_POLICY_HORIZONTAL);
			} else {
				if (policy == MX_SCROLL_POLICY_HORIZONTAL)
					mx_scroll_view_set_scroll_policy(MX_SCROLL_VIEW(instance), MX_SCROLL_POLICY_NONE);
				else if (policy == MX_SCROLL_POLICY_BOTH)
					mx_scroll_view_set_scroll_policy(MX_SCROLL_VIEW(instance), MX_SCROLL_POLICY_VERTICAL);
			}
		}
	}

	Handle<Value> ScrollView::PolicyVerticalGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;
		MxScrollPolicy policy;

		ClutterActor *instance = ObjectWrap::Unwrap<ScrollView>(info.This())->_actor;

		policy = mx_scroll_view_get_scroll_policy(MX_SCROLL_VIEW(instance));
		if (policy == MX_SCROLL_POLICY_VERTICAL || policy == MX_SCROLL_POLICY_BOTH) {
			return scope.Close(Boolean::New(True));
		}

		return scope.Close(Boolean::New(False));
	}

	void ScrollView::PolicyVerticalSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;
		MxScrollPolicy policy;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<ScrollView>(info.This())->_actor;

			policy = mx_scroll_view_get_scroll_policy(MX_SCROLL_VIEW(instance));
			if (value->ToBoolean()->Value()) {
				if (policy == MX_SCROLL_POLICY_HORIZONTAL)
					mx_scroll_view_set_scroll_policy(MX_SCROLL_VIEW(instance), MX_SCROLL_POLICY_BOTH);
				else if (policy == MX_SCROLL_POLICY_NONE)
					mx_scroll_view_set_scroll_policy(MX_SCROLL_VIEW(instance), MX_SCROLL_POLICY_VERTICAL);
			} else {
				if (policy == MX_SCROLL_POLICY_VERTICAL)
					mx_scroll_view_set_scroll_policy(MX_SCROLL_VIEW(instance), MX_SCROLL_POLICY_NONE);
				else if (policy == MX_SCROLL_POLICY_BOTH)
					mx_scroll_view_set_scroll_policy(MX_SCROLL_VIEW(instance), MX_SCROLL_POLICY_HORIZONTAL);
			}
		}
	}
}
