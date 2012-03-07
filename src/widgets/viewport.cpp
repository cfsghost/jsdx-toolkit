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
#include "viewport.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Viewport::Viewport() : Bin() {
		MxAdjustment *hadjust, *vadjust;

		_actor = mx_viewport_new();

		/* Elesticity */
		mx_scrollable_get_adjustments(MX_SCROLLABLE(_actor), &hadjust, &vadjust);
		mx_adjustment_set_elastic(hadjust, TRUE);
		mx_adjustment_set_elastic(vadjust, TRUE);
	}

	void Viewport::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Viewport");

		/* Methods */
		Bin::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> Viewport::New(const Arguments& args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Viewport* obj = new Viewport();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

}
