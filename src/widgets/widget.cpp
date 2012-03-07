#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../clutter.hpp"
#include "../actor.hpp"
#include "widget.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Widget::Widget() : Actor() {}

	void Widget::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Widget");

		/* Methods */
		Widget::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> Widget::New(const Arguments& args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Widget* obj = new Widget();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

}
