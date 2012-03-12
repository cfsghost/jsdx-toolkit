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
#include "dialog.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Dialog::Dialog() : Widget() {
		_actor = mx_dialog_new();
	}

	void Dialog::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Dialog");

		/* Methods */
		Dialog::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Dialog::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);
		Bin::PrototypeMethodsInit(constructor_template);

		/* Methods */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setTransientParent", Dialog::SetTransientParent);
	}

	/* ECMAScript constructor */
	Handle<Value> Dialog::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Dialog* obj = new Dialog();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	/* Methods */
	Handle<Value> Dialog::SetTransientParent(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsObject()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Dialog>(args.This())->_actor;
			ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

			mx_dialog_set_transient_parent(MX_DIALOG(instance), CLUTTER_ACTOR(actor));
		}

		return args.This();
	}

}
