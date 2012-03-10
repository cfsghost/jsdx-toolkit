#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#if ENABLE_MX
	#include <mx/mx.h>
#endif

#include "clutter.hpp"
#include "window.hpp"
#include "actor.hpp"
#include "stage.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> Window::constructor;

	Window::Window() : ObjectWrap()
	{
		/* Create a new stage */
		StageObject = Stage::New();

#if ENABLE_MX
		_window = mx_window_new_with_clutter_stage(CLUTTER_STAGE(ObjectWrap::Unwrap<Actor>(StageObject)->_actor));
#endif
	}

	void Window::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		Local<String> name = String::NewSymbol("Window");

		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		/* Methods */
		Window::PrototypeMethodsInit(constructor);

		target->Set(name, constructor->GetFunction());
	}

	void Window::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;
	}

	Local<Object> Window::New(void)
	{
		Local<Object> ObjectInstance = Window::constructor->GetFunction()->NewInstance();
		Window* obj = new Window();
		obj->Wrap(ObjectInstance);

		return ObjectInstance;
	}

	/* ECMAScript constructor */
	Handle<Value> Window::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Window* obj = new Window();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}
}
