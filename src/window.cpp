/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>

#if USE_X11
#include <clutter/x11/clutter-x11.h>
#include "backend/x11.hpp"
#endif

#include <clutter/clutter.h>
#if ENABLE_MX
	#include <mx/mx.h>
#endif

#include "jsdx_toolkit.hpp"
#include "actor.hpp"
#include "stage.hpp"
#include "window.hpp"

#if ENABLE_MX
#include "widgets/style.hpp"
#endif

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> JSDXWindow::constructor;

	JSDXWindow::JSDXWindow() : Stage()
	{
		/* Create a new stage */
		_actor = clutter_stage_new();
		clutter_stage_set_title(CLUTTER_STAGE(_actor), "Default");

#if ENABLE_MX
		_window = mx_window_new_with_clutter_stage(CLUTTER_STAGE(_actor));

		/* No toolbar by default */
		mx_window_set_has_toolbar(MX_WINDOW(_window), FALSE);
#endif

#if USE_X11
		hasDecorator = TRUE;
#endif
	}

	void JSDXWindow::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		Local<String> name = String::NewSymbol("Window");

		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		/* Methods */
		JSDXWindow::PrototypeMethodsInit(constructor);

		target->Set(name, constructor->GetFunction());
	}

	void JSDXWindow::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Stage::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("width"), JSDXWindow::WidthGetter, JSDXWindow::WidthSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("height"), JSDXWindow::HeightGetter, JSDXWindow::HeightSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("hasToolbar"), JSDXWindow::HasToolbarGetter, JSDXWindow::HasToolbarSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("rotation"), JSDXWindow::RotationGetter, JSDXWindow::RotationSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("hasDecorator"), JSDXWindow::HasDecoratorGetter, JSDXWindow::HasDecoratorSetter);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_setChild", JSDXWindow::SetChild);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "show", JSDXWindow::Show);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "showAll", JSDXWindow::ShowAll);
	}

	Local<Object> JSDXWindow::New(void)
	{
		Local<Object> ObjectInstance = JSDXWindow::constructor->GetFunction()->NewInstance();
		JSDXWindow* obj = new JSDXWindow();
		obj->Wrap(ObjectInstance);

		return ObjectInstance;
	}

	/* ECMAScript constructor */
	Handle<Value> JSDXWindow::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		JSDXWindow* obj = new JSDXWindow();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	/* Accessors */
	Handle<Value> JSDXWindow::WidthGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		JSDXWindow *window = ObjectWrap::Unwrap<JSDXWindow>(info.This());

#if ENABLE_MX
		gint width, height;
		mx_window_get_window_size(MX_WINDOW(window->_window), &width, &height);

		return scope.Close(Number::New(width));
#else
		Actor::WidthGetter(name, info);
#endif
	}

	void JSDXWindow::WidthSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			JSDXWindow *window = ObjectWrap::Unwrap<JSDXWindow>(info.This());

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

	Handle<Value> JSDXWindow::HeightGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		JSDXWindow *window = ObjectWrap::Unwrap<JSDXWindow>(info.This());

#if ENABLE_MX
		gint width, height;
		mx_window_get_window_size(MX_WINDOW(window->_window), &width, &height);

		return scope.Close(Number::New(height));
#else
		Actor::WidthGetter(name, info);
#endif
	}

	void JSDXWindow::HeightSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			JSDXWindow *window = ObjectWrap::Unwrap<JSDXWindow>(info.This());

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

	Handle<Value> JSDXWindow::HasToolbarGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

#if ENABLE_MX
		MxWindow *instance = ObjectWrap::Unwrap<JSDXWindow>(info.This())->_window;

		return scope.Close(
			Boolean::New(mx_window_get_has_toolbar(MX_WINDOW(instance)))
		);
#else
		return scope.Close(Boolean::New(False));
#endif
	}

	void JSDXWindow::HasToolbarSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

#if ENABLE_MX
		if (value->IsBoolean()) {
			MxWindow *instance = ObjectWrap::Unwrap<JSDXWindow>(info.This())->_window;

			mx_window_set_has_toolbar(MX_WINDOW(instance), value->ToBoolean()->Value());
		}
#endif
	}

	Handle<Value> JSDXWindow::HasDecoratorGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

#if USE_X11
		JSDXWindow *window = ObjectWrap::Unwrap<JSDXWindow>(info.This());

		return scope.Close(Boolean::New(window->hasDecorator));
#else
		return scope.Close(Boolean::New(True));
#endif

	}

	void JSDXWindow::HasDecoratorSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

#if USE_X11
		if (value->IsBoolean()) {
			JSDXWindow *window = ObjectWrap::Unwrap<JSDXWindow>(info.This());
			ClutterActor *actor = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			window->hasDecorator = value->ToBoolean()->Value();

			/* If window was realized, take off its decorator now */
			if (clutter_actor_get_flags(actor) & CLUTTER_ACTOR_REALIZED) {
				Window w = clutter_x11_get_stage_window(CLUTTER_STAGE(actor));
				Display *disp = clutter_x11_get_default_display();

				X11::setWindowDecorator(disp, w, window->hasDecorator);
			}
		}
#endif

	}

	Handle<Value> JSDXWindow::RotationGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

#if ENABLE_MX
		MxWindow *instance = ObjectWrap::Unwrap<JSDXWindow>(info.This())->_window;

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

	void JSDXWindow::RotationSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

#if ENABLE_MX
		if (value->IsNumber()) {
			MxWindow *instance = ObjectWrap::Unwrap<JSDXWindow>(info.This())->_window;

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
	Handle<Value> JSDXWindow::SetChild(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

#if ENABLE_MX
		MxWindow *instance = ObjectWrap::Unwrap<JSDXWindow>(args.This())->_window;

		mx_window_set_child(MX_WINDOW(instance), CLUTTER_ACTOR(actor));
#else
		Container::Add(args);
#endif

		return args.This();
	}

	Handle<Value> JSDXWindow::Show(const Arguments &args)
	{
		HandleScope scope;

		JSDXWindow *window = ObjectWrap::Unwrap<JSDXWindow>(args.This());
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		clutter_actor_show(actor);

		/* Set Window Decorator */
		Window w = clutter_x11_get_stage_window(CLUTTER_STAGE(actor));
		Display *disp = clutter_x11_get_default_display();
		X11::setWindowDecorator(disp, w, window->hasDecorator);

		return args.This();
	}

	Handle<Value> JSDXWindow::ShowAll(const Arguments &args)
	{
		HandleScope scope;

		JSDXWindow *window = ObjectWrap::Unwrap<JSDXWindow>(args.This());
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		clutter_actor_show_all(actor);

		/* Set Window Decorator */
		Window w = clutter_x11_get_stage_window(CLUTTER_STAGE(actor));
		Display *disp = clutter_x11_get_default_display();
		X11::setWindowDecorator(disp, w, window->hasDecorator);


		return args.This();
	}
}
