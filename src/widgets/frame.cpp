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
#include "frame.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Frame::Frame() : Widget() {
		_actor = mx_frame_new();
	}

	void Frame::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Frame");

		/* Methods */
		Frame::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Frame::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Bin::PrototypeMethodsInit(constructor_template);
	}

	/* ECMAScript constructor */
	Handle<Value> Frame::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Frame* obj = new Frame();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

}
