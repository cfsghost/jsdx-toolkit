#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <mx/mx.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../jsdx_toolkit.hpp"
#include "../actor.hpp"
#include "bin.hpp"
#include "scrollable.hpp"
#include "viewport.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> Viewport::constructor;

	Viewport::Viewport() : Bin() {
		_actor = mx_viewport_new();

		mx_viewport_set_sync_adjustments(MX_VIEWPORT(_actor), TRUE);
	}

	void Viewport::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		/* Template */
		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		Local<String> name = String::NewSymbol("Viewport");

		/* Constructor */
		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		/* Methods */
		Widget::PrototypeMethodsInit(constructor);
		Bin::PrototypeMethodsInit(constructor);

		target->Set(name, constructor->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> Viewport::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Viewport* obj = new Viewport();
		obj->Wrap(args.This());

		/* Initializing sub-objects */
		Local<Object> ScrollableObject = Scrollable::New(MX_SCROLLABLE(obj->_actor));
		args.Holder()->Set(String::NewSymbol("scroll"), ScrollableObject);

		return scope.Close(args.This());
	}

}
