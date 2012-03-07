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
#include "kinetic_scrollview.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	KineticScrollView::KineticScrollView() : Bin() {
		_actor = mx_kinetic_scroll_view_new();

		mx_kinetic_scroll_view_set_overshoot((MxKineticScrollView *)_actor, 0.2);
	}

	void KineticScrollView::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("KineticScrollView");

		/* Methods */
		Bin::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> KineticScrollView::New(const Arguments& args)
	{
		HandleScope scope;
		static ClutterColor color;

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

}
