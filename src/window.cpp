#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#if ENABLE_MX
	#include <mx/mx.h>
#endif

#include "clutter.hpp"
#include "actor.hpp"
#include "stage.hpp"
#include "window.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> Window::constructor;

	Window::Window() : Stage()
	{
		/* Create a new stage */
		_actor = clutter_stage_new();
		clutter_stage_set_title(CLUTTER_STAGE(_actor), "Default");

#if ENABLE_MX
		_window = mx_window_new_with_clutter_stage(CLUTTER_STAGE(_actor));
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

		Stage::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("hasToolbar"), Window::HasToolbarGetter, Window::HasToolbarSetter);
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

	/* Accessors */
	Handle<Value> Window::HasToolbarGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

#if ENABLE_MX
		MxWindow *instance = ObjectWrap::Unwrap<Window>(info.This())->_window;

		return scope.Close(
			Boolean::New(mx_window_get_has_toolbar(MX_WINDOW(instance)))
		);
#else
		return scope.Close(Boolean::New(False));
#endif
	}

	void Window::HasToolbarSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

#if ENABLE_MX
		if (value->IsBoolean()) {
			MxWindow *instance = ObjectWrap::Unwrap<Window>(info.This())->_window;

			mx_window_set_has_toolbar(MX_WINDOW(instance), value->ToBoolean()->Value());
		}
#endif
	}
}