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

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> ScrollView::New(const Arguments& args)
	{
		HandleScope scope;
		static ClutterColor color;

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

}
