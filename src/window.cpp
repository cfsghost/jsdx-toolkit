/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#if ENABLE_MX
	#include <mx/mx.h>
#endif

#include "jsdx_toolkit.hpp"
#include "actor.hpp"
#include "stage.hpp"
#include "window.hpp"

namespace JSDXToolkit {
 
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

		/* No toolbar by default */
		mx_window_set_has_toolbar(MX_WINDOW(_window), FALSE);
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

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("width"), Window::WidthGetter, Window::WidthSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("height"), Window::HeightGetter, Window::HeightSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("hasToolbar"), Window::HasToolbarGetter, Window::HasToolbarSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("rotation"), Window::RotationGetter, Window::RotationSetter);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setChild", Window::SetChild);
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
	Handle<Value> Window::WidthGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Window *window = ObjectWrap::Unwrap<Window>(info.This());

#if ENABLE_MX
		gint width, height;
		mx_window_get_window_size(MX_WINDOW(window->_window), &width, &height);

		return scope.Close(Number::New(width));
#else
		Actor::WidthGetter(name, info);
#endif
	}

	void Window::WidthSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			Window *window = ObjectWrap::Unwrap<Window>(info.This());

#if ENABLE_MX
			gint width, height;
			mx_window_get_window_size(MX_WINDOW(window->_window), &width, &height);

			width = value->ToInteger()->Value();

			mx_window_set_window_size(MX_WINDOW(window->_window), width, height);
#else
		Actor::WidthSetter(name, info);
#endif
		}
	}

	Handle<Value> Window::HeightGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Window *window = ObjectWrap::Unwrap<Window>(info.This());

#if ENABLE_MX
		gint width, height;
		mx_window_get_window_size(MX_WINDOW(window->_window), &width, &height);

		return scope.Close(Number::New(height));
#else
		Actor::WidthGetter(name, info);
#endif
	}

	void Window::HeightSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			Window *window = ObjectWrap::Unwrap<Window>(info.This());

#if ENABLE_MX
			gint width, height;
			mx_window_get_window_size(MX_WINDOW(window->_window), &width, &height);

			height = value->ToInteger()->Value();

			mx_window_set_window_size(MX_WINDOW(window->_window), width, height);
#else
		Actor::WidthSetter(name, info);
#endif
		}
	}

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

	Handle<Value> Window::RotationGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

#if ENABLE_MX
		MxWindow *instance = ObjectWrap::Unwrap<Window>(info.This())->_window;

		switch(mx_window_get_window_rotation(MX_WINDOW(instance))) {
		case MX_WINDOW_ROTATION_0:
			return scope.Close(Integer::New(0));
			break;

		case MX_WINDOW_ROTATION_90:
			return scope.Close(Integer::New(90));
			break;

		case MX_WINDOW_ROTATION_180:
			return scope.Close(Integer::New(180));
			break;

		case MX_WINDOW_ROTATION_270:
			return scope.Close(Integer::New(270));
		}
#else
		return scope.Close(Integer::New(0));
#endif
	}

	void Window::RotationSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

#if ENABLE_MX
		if (value->IsNumber()) {
			MxWindow *instance = ObjectWrap::Unwrap<Window>(info.This())->_window;

			switch(value->ToInteger()->Value()) {
			case 0:
				mx_window_set_window_rotation(MX_WINDOW(instance), MX_WINDOW_ROTATION_0);
				break;

			case 90:
				mx_window_set_window_rotation(MX_WINDOW(instance), MX_WINDOW_ROTATION_90);
				break;

			case 180:
				mx_window_set_window_rotation(MX_WINDOW(instance), MX_WINDOW_ROTATION_180);
				break;

			case 270:
				mx_window_set_window_rotation(MX_WINDOW(instance), MX_WINDOW_ROTATION_270);
				break;
			}
		}
#endif
	}

	/* Methods */
	Handle<Value> Window::SetChild(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

#if ENABLE_MX
		MxWindow *instance = ObjectWrap::Unwrap<Window>(args.This())->_window;

		mx_window_set_child(MX_WINDOW(instance), CLUTTER_ACTOR(actor));
#else
		Container::Add(args);
#endif

		return args.This();
	}
}
