#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../clutter.hpp"
#include "../actor.hpp"
#include "../container.hpp"
#include "widget.hpp"
#include "bin.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Bin::Bin() : Widget() {}

	void Bin::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Bin");

		/* Methods */
		Bin::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Bin::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		/* Inherit methods from Actor */
		Container::PrototypeMethodsInit(constructor_template);
	}

	/* ECMAScript constructor */
	Handle<Value> Bin::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Bin* obj = new Bin();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

}
