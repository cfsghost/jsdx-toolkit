#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#if ENABLE_MX
	#include <mx/mx.h>
#endif

#include "clutter.hpp"
#include "application.hpp"
#include "window.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Application::Application(const char *name) : ObjectWrap()
	{
#if ENABLE_MX
		_application = mx_application_new(NULL, NULL, name, (MxApplicationFlags)0);
#else
		clutter_init(NULL, NULL);
#endif
	}

	void Application::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Application");

		/* Methods */
		Application::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Application::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "run", Application::Run);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "createWindow", Application::CreateWindow);
	}

	/* ECMAScript constructor */
	Handle<Value> Application::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		if (!args[0]->IsString()) {
			return ThrowException(Exception::TypeError(
				String::New("first argument must be string")));
		}

		// Creates a new instance object of this type and wraps it.
		Application* obj = new Application(*String::Utf8Value(args[0]->ToString()));
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> Application::Run(const Arguments &args)
	{
		Application *application = ObjectWrap::Unwrap<Application>(args.This());

		ev_ref(EV_DEFAULT_UC);

		return Undefined();
	}

	Handle<Value> Application::CreateWindow(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsFunction()) {
			Application *application = ObjectWrap::Unwrap<Application>(args.This());
			Persistent<Function> *callback = new Persistent<Function>();
			*callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));

			/* Create a new window */
			Local<Object> WindowObject = Window::New();
			Window *window = ObjectWrap::Unwrap<Window>(WindowObject);
			mx_application_add_window(application->_application, window->_window);

			/* Prepare arguments */
			Local<Value> argv[1] = {
				scope.Close(WindowObject)
			};

			(*callback)->Call(Context::GetCurrent()->Global(), 1, argv);
		}

		return args.This();
	}
}
