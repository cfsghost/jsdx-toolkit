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
#include "button.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Button::Button() : Widget() {
		_actor = mx_button_new();
	}

	void Button::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Button");

		/* Methods */
		Button::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Button::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);
		Bin::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("label"), Button::LabelGetter, Button::LabelSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> Button::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Button* obj = new Button();
		obj->Wrap(args.This());

		if (args[0]->IsString()) {
			mx_button_set_label(MX_BUTTON(obj->_actor), *String::Utf8Value(args[0]->ToString()));
		}

		return scope.Close(args.This());
	}

	Handle<Value> Button::LabelGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Button>(info.This())->_actor;

		return scope.Close(
			String::New(mx_button_get_label(MX_BUTTON(instance)))
		);
	}

	void Button::LabelSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Button>(info.This())->_actor;

			mx_button_set_label(MX_BUTTON(instance), *String::Utf8Value(value->ToString()));
		}
	}

}
