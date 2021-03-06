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

#if USE_X11
#include <clutter/x11/clutter-x11.h>
#endif

#include <string.h>

#include "jsdx_toolkit.hpp"
#include "actor.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	struct PropertyDefine properties[] = {
		{ "anchor-x", G_TYPE_FLOAT },
		{ "anchor-y", G_TYPE_FLOAT },
		{ "depth", G_TYPE_FLOAT },
		{ "fixed-x", G_TYPE_FLOAT },
		{ "fixed-y", G_TYPE_FLOAT },
		{ "height", G_TYPE_FLOAT },
		{ "min-height", G_TYPE_FLOAT },
		{ "min-width", G_TYPE_FLOAT },
		{ "natural-height", G_TYPE_FLOAT },
		{ "natural-width", G_TYPE_FLOAT },
		{ "opacity", G_TYPE_UINT },
		{ "rotation-angle-x", G_TYPE_DOUBLE },
		{ "rotation-angle-y", G_TYPE_DOUBLE },
		{ "rotation-angle-z", G_TYPE_DOUBLE },
		{ "scale-center-x", G_TYPE_FLOAT },
		{ "scale-center-y", G_TYPE_FLOAT },
		{ "scale-x", G_TYPE_DOUBLE },
		{ "scale-y", G_TYPE_DOUBLE },
		{ "width", G_TYPE_FLOAT },
		{ "x", G_TYPE_FLOAT },
		{ "y", G_TYPE_FLOAT },
		{ NULL, 0 },
	};

	struct EventDefine events[] = {
		{ JSDX_TOOLKIT_EVENT_DESTROY, "destroy" },
		{ JSDX_TOOLKIT_EVENT_SHOW, "show" },
		{ JSDX_TOOLKIT_EVENT_HIDE, "hide" },
		{ JSDX_TOOLKIT_EVENT_PRESS, "press" },
		{ JSDX_TOOLKIT_EVENT_RELEASE, "release" },
		{ JSDX_TOOLKIT_EVENT_KEY_PRESS, "keypress" },
		{ JSDX_TOOLKIT_EVENT_KEY_RELEASE, "keyrelease" },
		{ JSDX_TOOLKIT_EVENT_CLICK, "click" },
		{ JSDX_TOOLKIT_EVENT_LONG_PRESS, "long_press" },
		{ JSDX_TOOLKIT_EVENT_ENTER, "enter" },
		{ JSDX_TOOLKIT_EVENT_LEAVE, "leave" },
		{ JSDX_TOOLKIT_EVENT_MOTION, "motion" },
		{ JSDX_TOOLKIT_EVENT_DRAG, "drag" },
		{ JSDX_TOOLKIT_EVENT_SCROLL, "scroll" },
		{ -1, NULL },
	};

	Persistent<FunctionTemplate> Actor::constructor_template;

	Actor::Actor()
		: ObjectWrap() {

		_actor = NULL;
		_animation = NULL;

		/* Callback function for events */
		destroy_cb = NULL;
		button_press_cb = NULL;
		button_release_cb = NULL;
		button_clicked_cb = NULL;
		key_press_cb = NULL;
		key_release_cb = NULL;
		long_press_cb = NULL;
		enter_cb = NULL;
		leave_cb = NULL;
		motion_cb = NULL;
		drag_cb = NULL;
		scroll_cb = NULL;
	}

	Actor::~Actor() {
		if (_actor) {
			if (_animation)
				clutter_actor_detach_animation((ClutterActor *)_actor);

			clutter_actor_destroy((ClutterActor *)_actor);
		}

		/* release callback function */
		delete destroy_cb;
		delete button_press_cb;
		delete button_release_cb;
		delete key_press_cb;
		delete key_release_cb;
		delete button_clicked_cb;
		delete long_press_cb;
		delete enter_cb;
		delete leave_cb;
		delete motion_cb;
	}

	void Actor::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		/* Create a new JavaScript class */
		Local<String> name = String::NewSymbol("Actor");
		constructor_template = Persistent<FunctionTemplate>::New(FunctionTemplate::New(New));
		constructor_template->SetClassName(name);

		PrototypeMethodsInit(constructor_template);

		target->Set(name, constructor_template->GetFunction());
	}

	void Actor::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("x"), Actor::XGetter, Actor::XSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("y"), Actor::YGetter, Actor::YSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("depth"), Actor::DepthGetter, Actor::DepthSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("opacity"), Actor::OpacityGetter, Actor::OpacitySetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("width"), Actor::WidthGetter, Actor::WidthSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("height"), Actor::HeightGetter, Actor::HeightSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("hasClip"), Actor::HasClipGetter, Actor::HasClipSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("reactive"), Actor::ReactiveGetter, Actor::ReactiveSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("marginTop"), Actor::MarginTopGetter, Actor::MarginTopSetter);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_destroy", Actor::Destroy);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "show", Actor::Show);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "showAll", Actor::ShowAll);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "hide", Actor::Hide);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "resize", Actor::Resize);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setPosition", Actor::SetPosition);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setBackgroundColor", Actor::SetBackgroundColor);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "raiseTop", Actor::RaiseTop);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "lowerBottom", Actor::LowerBottom);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "clone", Actor::Clone);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "isVisible", Actor::IsVisible);

		/* Anchor */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setAnchorFromGravity", Actor::SetAnchorFromGravity);

		/* Scale */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "scale", Actor::Scale);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setScaleGravity", Actor::SetScaleGravity);

		/* Rotation */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "rotate", Actor::Rotate);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setRotationGravity", Actor::SetRotationGravity);

		/* Event handler */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "on", Actor::On);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_off", Actor::Off);

		/* Effect */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "effect", Actor::Effect);

		/* Animation */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "animate", Actor::Animate);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setAnimate", Actor::SetAnimate);

		/* Constraint */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "clearConstraints", Actor::ClearConstraints);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setAlignConstraint", Actor::SetAlignConstraint);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setBindConstraint", Actor::SetBindConstraint);

	}

	bool Actor::PropertyValueInit(GValue *gvalue, Handle<Value> property, Handle<Value> value)
	{
		int i;
		bool ret = False;

		for (i = 0; properties[i].name != NULL; ++i) {
			if (strcmp(properties[i].name, *String::AsciiValue(property->ToString())) == 0) {
				g_value_init(gvalue, properties[i].type);

				switch(properties[i].type) {
				case G_TYPE_UINT:
					if (!value->IsNumber())
						break;

					g_value_set_uint(gvalue, value->ToInteger()->Value());
					ret = True;

					break;

				case G_TYPE_FLOAT:
					if (!value->IsNumber())
						break;

					g_value_set_float(gvalue, value->NumberValue());
					ret = True;

					break;

				case G_TYPE_DOUBLE:
					if (!value->IsNumber())
						break;

					g_value_set_double(gvalue, value->NumberValue());
					ret = True;

					break;

				}
			}
		}

		return ret;
	}

	Handle<Value> Actor::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Actor* obj = new Actor();
		obj->Wrap(args.This());

		return args.This();
	}

	Handle<Value> Actor::Destroy(const Arguments &args)
	{
		HandleScope scope;
		Actor *obj = ObjectWrap::Unwrap<Actor>(args.This());
		clutter_actor_destroy((ClutterActor *)obj->_actor);
		obj->_actor = NULL;
		obj->_animation = NULL;

//		obj->Unref();

		return Undefined();
	}

	Handle<Value> Actor::Show(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		clutter_actor_show(instance);

		return args.This();
	}

	Handle<Value> Actor::ShowAll(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		clutter_actor_show_all(instance);

		return args.This();
	}

	Handle<Value> Actor::Hide(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		clutter_actor_hide(instance);

		return args.This();
	}

	Handle<Value> Actor::Resize(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsNumber() && args[1]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_actor_set_size(CLUTTER_ACTOR(instance), args[0]->NumberValue(), args[1]->NumberValue());
		}

		return args.This();
	}

	Handle<Value> Actor::WidthGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Number::New(clutter_actor_get_width(CLUTTER_ACTOR(instance)))
		);
	}

	void Actor::WidthSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_width(CLUTTER_ACTOR(instance), value->NumberValue());
		}
	}

	Handle<Value> Actor::HeightGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Number::New(clutter_actor_get_height(CLUTTER_ACTOR(instance)))
		);
	}

	void Actor::HeightSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_height(CLUTTER_ACTOR(instance), value->NumberValue());
		}
	}

	Handle<Value> Actor::SetPosition(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsNumber() && args[1]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_actor_set_position(CLUTTER_ACTOR(instance), args[0]->NumberValue(), args[1]->NumberValue());
		}

		return args.This();
	}

	Handle<Value> Actor::SetBackgroundColor(const Arguments &args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			color.red = args[0]->NumberValue();
			color.green = args[1]->NumberValue();
			color.blue = args[2]->NumberValue();
			color.alpha = args[3]->NumberValue();

			clutter_actor_set_background_color(CLUTTER_ACTOR(instance), &color);
		}

		return args.This();
	}

	Handle<Value> Actor::RaiseTop(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		ClutterActor *parent = clutter_actor_get_parent(instance);

		clutter_actor_set_child_above_sibling(parent, CLUTTER_ACTOR(instance), NULL);

		return args.This();
	}

	Handle<Value> Actor::LowerBottom(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		ClutterActor *parent = clutter_actor_get_parent(instance);

		clutter_actor_set_child_below_sibling(parent, CLUTTER_ACTOR(instance), NULL);

		clutter_actor_queue_relayout(parent);

		return args.This();
	}

	Handle<Value> Actor::Clone(const Arguments &args)
	{
		HandleScope scope;

		Actor *actor = ObjectWrap::Unwrap<Actor>(args.This());
		ClutterActor *instance = actor->_actor;
		if (instance)
			return args.This();

		if (args[0]->IsObject()) {

			Actor *source = ObjectWrap::Unwrap<Actor>(args[0]->ToObject());
			if (!source->_actor)
				return args.This();

			actor->_actor = clutter_clone_new(source->_actor);
		}

		return args.This();
	}

	Handle<Value> Actor::IsVisible(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		return scope.Close(Boolean::New(CLUTTER_ACTOR_IS_VISIBLE(instance)));
	}

	/* Accessor */
	Handle<Value> Actor::XGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Number::New(clutter_actor_get_x(CLUTTER_ACTOR(instance)))
		);
	}

	Handle<Value> Actor::YGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Number::New(clutter_actor_get_y(CLUTTER_ACTOR(instance)))
		);
	}

	void Actor::XSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_x(CLUTTER_ACTOR(instance), value->NumberValue());
		}
	}

	void Actor::YSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_y(CLUTTER_ACTOR(instance), value->NumberValue());
		}
	}

	Handle<Value> Actor::OpacityGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Integer::New(clutter_actor_get_opacity(CLUTTER_ACTOR(instance)))
		);
	}

	void Actor::OpacitySetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_opacity(CLUTTER_ACTOR(instance), value->ToInteger()->Value());
		}
	}

	Handle<Value> Actor::DepthGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Number::New(clutter_actor_get_depth(CLUTTER_ACTOR(instance)))
		);
	}

	void Actor::DepthSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_depth(CLUTTER_ACTOR(instance), value->NumberValue());
		}
	}

	Handle<Value> Actor::HasClipGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Boolean::New(clutter_actor_has_clip(CLUTTER_ACTOR(instance)))
		);
	}

	void Actor::HasClipSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		/* Do nothing */
	}

	Handle<Value> Actor::ReactiveGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Boolean::New(clutter_actor_get_reactive(CLUTTER_ACTOR(instance)))
		);
	}

	void Actor::ReactiveSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_reactive(CLUTTER_ACTOR(instance), value->ToBoolean()->Value());
		}
	}

	Handle<Value> Actor::MarginTopGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Integer::New(clutter_actor_get_margin_top(CLUTTER_ACTOR(instance)))
		);
	}

	void Actor::MarginTopSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_margin_top(CLUTTER_ACTOR(instance), value->ToInteger()->Value());
		}
	}

	Handle<Value> Actor::SetAnchorFromGravity(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_actor_set_anchor_point_from_gravity(CLUTTER_ACTOR(instance), (ClutterGravity)args[0]->ToInteger()->Value());
		}

		return args.This();
	}

	Handle<Value> Actor::Scale(const Arguments &args)
	{
		HandleScope scope;
		static gdouble scale_x, scale_y;
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (args[0]->IsUndefined() || args[0]->IsNull()) {
			Local<Object> o;

			clutter_actor_get_scale(CLUTTER_ACTOR(instance), &scale_x, &scale_y);

			/* create a JavaScript Object */
			o = Object::New();
			o->Set(String::New("scale_x"), Number::New(scale_x));
			o->Set(String::New("scale_y"), Number::New(scale_y));

			return scope.Close(o);
		}
		
		if (args[0]->IsNumber() && args[1]->IsNumber()) {
			if (args.Length() == 2) {
				clutter_actor_set_scale(CLUTTER_ACTOR(instance), args[0]->NumberValue(), args[1]->NumberValue());

			} else if (args.Length() == 3) {
				if (args[2]->IsNumber())
					clutter_actor_set_scale_with_gravity(CLUTTER_ACTOR(instance),
						(gfloat)args[0]->NumberValue(),
						(gfloat)args[1]->NumberValue(), 
						(ClutterGravity)args[2]->ToInteger()->Value());

			} else if (args.Length() == 4) {
				if (args[2]->IsNumber() && args[3]->IsNumber())
					clutter_actor_set_scale_full(CLUTTER_ACTOR(instance),
						(gfloat)args[0]->NumberValue(),
						(gfloat)args[1]->NumberValue(), 
						args[2]->NumberValue(), 
						args[3]->NumberValue()); 

			}
		}

		return args.This();
	}

	Handle<Value> Actor::SetScaleGravity(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			g_object_set(G_OBJECT(instance), "scale-gravity", args[0]->ToInteger()->Value(), NULL);
		}

		return args.This();
	}

	Handle<Value> Actor::Rotate(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (args.Length() == 2) {
			if (args[0]->IsNumber() && args[1]->IsNumber()) {
				clutter_actor_set_z_rotation_from_gravity(CLUTTER_ACTOR(instance),
						args[0]->NumberValue(), 
						(ClutterGravity)args[1]->ToInteger()->Value());
			}

		} else if (args.Length() == 5) {
			if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber() && args[4]->IsNumber()) {
				clutter_actor_set_rotation(CLUTTER_ACTOR(instance),
						(ClutterRotateAxis)args[0]->ToInteger()->Value(),
						args[1]->NumberValue(), 
						args[2]->NumberValue(), 
						args[3]->NumberValue(), 
						args[4]->NumberValue()); 

			}
		}

		return args.This();
	}

	Handle<Value> Actor::SetRotationGravity(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			g_object_set(G_OBJECT(instance), "rotation-center-z-gravity", (ClutterGravity)args[0]->ToInteger()->Value(), NULL);
		}

		return args.This();
	}

	/* Event Dispatcher */
	void Actor::_DestroyCallback(ClutterActor *actor, gpointer user_data)
	{
		NodeCallback *cb = (NodeCallback *)user_data;

		cb->cb->Call(cb->Holder, 0, NULL);
	}

	gboolean Actor::_PressCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("button"), Number::New(event->button.button));

		/* Get coordinates on actor */
		gfloat actor_x, actor_y;
		clutter_actor_transform_stage_point(actor, event->button.x, event->button.y, &actor_x, &actor_y);
		o->Set(String::New("x"), Number::New(actor_x));
		o->Set(String::New("y"), Number::New(actor_y));

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_PRESS)),
			o
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}

	gboolean Actor::_ReleaseCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("button"), Number::New(event->button.button));

		/* Get coordinates on actor */
		gfloat actor_x, actor_y;
		clutter_actor_transform_stage_point(actor, event->button.x, event->button.y, &actor_x, &actor_y);
		o->Set(String::New("x"), Number::New(actor_x));
		o->Set(String::New("y"), Number::New(actor_y));

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_RELEASE)),
			o
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}

	gboolean Actor::_KeyPressCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("keyval"), Number::New(event->key.keyval));

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_KEY_PRESS)),
			o
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}

	gboolean Actor::_KeyReleaseCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("keyval"), Number::New(event->key.keyval));

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_KEY_RELEASE)),
			o
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}


	void Actor::_ClickActionCallback(ClutterClickAction *action, ClutterActor *actor, gpointer user_data)
	{
		const unsigned argc = 1;
		NodeCallback *cb = (NodeCallback *)user_data;

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_CLICK))
		};

		cb->cb->Call(cb->Holder, argc, argv);
	}

	gboolean Actor::_LongPressActionCallback(ClutterClickAction *action, ClutterActor *actor, ClutterLongPressState state, gpointer user_data)
	{
		const unsigned argc = 2;
		gfloat x, y;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* Get position */
		clutter_click_action_get_coords(action, &x, &y);

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("state"), Integer::New(state));
		o->Set(String::New("x"), Number::New(x));
		o->Set(String::New("y"), Number::New(y));

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_LONG_PRESS)),
			o
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}

	gboolean Actor::_EnterCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		const unsigned argc = 1;
		NodeCallback *cb = (NodeCallback *)user_data;

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_ENTER))
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}

	gboolean Actor::_LeaveCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		const unsigned argc = 1;
		NodeCallback *cb = (NodeCallback *)user_data;

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_LEAVE))
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}

	gboolean Actor::_MotionCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("time"), Uint32::New(event->motion.time));

		/* Get coordinates on actor */
		gfloat actor_x, actor_y;
		clutter_actor_transform_stage_point(actor, event->motion.x, event->motion.y, &actor_x, &actor_y);
		o->Set(String::New("x"), Number::New(actor_x));
		o->Set(String::New("y"), Number::New(actor_y));

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_MOTION)),
			o
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}

	void Actor::_DragActionBeginCallback(ClutterClickAction *action,
		ClutterActor *actor,
		gfloat event_x,
		gfloat event_y,
		ClutterModifierType modifiers,
		gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("x"), Number::New(event_x));
		o->Set(String::New("y"), Number::New(event_y));
		o->Set(String::New("y"), Integer::New(modifiers));

		Local<Value> argv[argc] = {
			Local<Value>::New(String::New("begin")),
			o
		};

		cb->cb->Call(cb->Holder, argc, argv);
	}

	void Actor::_DragActionEndCallback(ClutterClickAction *action,
		ClutterActor *actor,
		gfloat event_x,
		gfloat event_y,
		ClutterModifierType modifiers,
		gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("x"), Number::New(event_x));
		o->Set(String::New("y"), Number::New(event_y));
		o->Set(String::New("modifier"), Integer::New(modifiers));

		Local<Value> argv[argc] = {
			Local<Value>::New(String::New("end")),
			o
		};

		cb->cb->Call(cb->Holder, argc, argv);
	}

	void Actor::_DragActionMotionCallback(ClutterClickAction *action,
		ClutterActor *actor,
		gfloat delta_x,
		gfloat delta_y,
		gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("delta_x"), Number::New(delta_x));
		o->Set(String::New("delta_y"), Number::New(delta_y));

		Local<Value> argv[argc] = {
			Local<Value>::New(String::New("motion")),
			o
		};

		cb->cb->Call(cb->Holder, argc, argv);
	}

	gboolean Actor::_ScrollCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		const unsigned argc = 2;
		NodeCallback *cb = (NodeCallback *)user_data;

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("x"), Number::New(event->scroll.x));
		o->Set(String::New("y"), Number::New(event->scroll.y));
		o->Set(String::New("time"), Uint32::New(event->scroll.time));
		o->Set(String::New("direction"), Uint32::New(event->scroll.direction));

		Local<Value> argv[argc] = {
			Local<Value>::New(Integer::New(JSDX_TOOLKIT_EVENT_SCROLL)),
			o
		};

		Local<Value> ret = cb->cb->Call(cb->Holder, argc, argv);

		return (*ret)->IsTrue();
	}

	Handle<Value> Actor::On(const Arguments &args)
	{
		HandleScope scope;
		Local<Value> Event;
		Local<Value> Options;
		Local<Value> Callback;
		Actor *obj = ObjectWrap::Unwrap<Actor>(args.This());

		ClutterAction *action;
		ClutterActor *instance = obj->_actor;

		/* Check arguments */
		if (args.Length() > 3) {
			return ThrowException(Exception::TypeError(
				String::New("too much arguments")));
		} else if (args.Length() < 2) {
			return ThrowException(Exception::TypeError(
				String::New("too few arguments")));
		}

		if (args.Length() == 2) {
			Event = args[0];
			Callback = args[1];
		} else if (args.Length() == 3) {
			Event = args[0];
			Options = args[1];
			Callback = args[2];

			if (!Options->IsObject())
				return ThrowException(Exception::TypeError(
					String::New("options parameter must be object")));
		}

		if (!Event->IsString())
			return ThrowException(Exception::TypeError(
				String::New("first arguments must be string")));

		if (!Callback->IsFunction())
			return ThrowException(Exception::TypeError(
				String::New("require callback function")));

		/* Get event id */
		int i;
		int event = -1;
		for (i = 0; events[i].event != NULL; ++i) {
			if (strcmp(events[i].event, *String::AsciiValue(Event->ToString())) == 0) {
				event = events[i].id;
				break;
			}
		}

		//switch(Event->ToInteger()->Value()) {
		bool reg = FALSE;
		switch(event) {
		case JSDX_TOOLKIT_EVENT_DESTROY:
			if (!obj->destroy_cb) {
				reg = TRUE;
				obj->destroy_cb = new NodeCallback();
			} else {
				obj->destroy_cb->Holder.Dispose();
				obj->destroy_cb->cb.Dispose();
			}

			obj->destroy_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->destroy_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "destroy", G_CALLBACK(Actor::_DestroyCallback), (gpointer)obj->destroy_cb);

			break;

		case JSDX_TOOLKIT_EVENT_PRESS:
			if (!obj->button_press_cb) {
				reg = TRUE;
				obj->button_press_cb = new NodeCallback();
			} else {
				obj->button_press_cb->Holder.Dispose();
				obj->button_press_cb->cb.Dispose();
			}

			obj->button_press_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->button_press_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "button-press-event", G_CALLBACK(Actor::_PressCallback), (gpointer)obj->button_press_cb);

			break;

		case JSDX_TOOLKIT_EVENT_RELEASE:
			if (!obj->button_release_cb) {
				reg = TRUE;
				obj->button_release_cb = new NodeCallback();
			} else {
				obj->button_release_cb->Holder.Dispose();
				obj->button_release_cb->cb.Dispose();
			}

			obj->button_release_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->button_release_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "button-release-event", G_CALLBACK(Actor::_ReleaseCallback), (gpointer)obj->button_release_cb);

			break;

		case JSDX_TOOLKIT_EVENT_KEY_PRESS:
			if (!obj->key_press_cb) {
				reg = TRUE;
				obj->key_press_cb = new NodeCallback();
			} else {
				obj->key_press_cb->Holder.Dispose();
				obj->key_press_cb->cb.Dispose();
			}

			obj->key_press_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->key_press_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "key-press-event", G_CALLBACK(Actor::_KeyPressCallback), (gpointer)obj->key_press_cb);

			break;

		case JSDX_TOOLKIT_EVENT_KEY_RELEASE:
			if (!obj->key_release_cb) {
				reg = TRUE;
				obj->key_release_cb = new NodeCallback();
			} else {
				obj->key_release_cb->Holder.Dispose();
				obj->key_release_cb->cb.Dispose();
			}

			obj->key_release_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->key_release_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "key-release-event", G_CALLBACK(Actor::_KeyReleaseCallback), (gpointer)obj->key_release_cb);

			break;

		case JSDX_TOOLKIT_EVENT_LONG_PRESS:
			action = clutter_click_action_new();
			clutter_actor_add_action(instance, action);

			if (!obj->long_press_cb) {
				reg = TRUE;
				obj->long_press_cb = new NodeCallback();
			} else {
				obj->long_press_cb->Holder.Dispose();
				obj->long_press_cb->cb.Dispose();
			}

			obj->long_press_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->long_press_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			/* Get duration, threshold */
			if (args.Length() == 3 && Options->IsObject()) {
				Local<Value> OptionValue;

				OptionValue = Options->ToObject()->Get(String::New("threshold"));
				if (OptionValue->IsNumber()) {
					g_object_set(G_OBJECT(action), "long-press-threshold", OptionValue->ToInteger()->Value(), NULL);
				}

				OptionValue = Options->ToObject()->Get(String::New("duration"));
				if (OptionValue->IsNumber()) {
					g_object_set(G_OBJECT(action), "long-press-duration", OptionValue->ToInteger()->Value(), NULL);
				}
			}

			if (reg)
				g_signal_connect(action, "long-press", G_CALLBACK(Actor::_LongPressActionCallback), (gpointer)obj->long_press_cb);

			break;

		case JSDX_TOOLKIT_EVENT_CLICK:
			action = clutter_click_action_new();
			clutter_actor_add_action(instance, action);

			if (!obj->button_clicked_cb) {
				reg = TRUE;
				obj->button_clicked_cb = new NodeCallback();
			} else {
				obj->button_clicked_cb->Holder.Dispose();
				obj->button_clicked_cb->cb.Dispose();
			}

			obj->button_clicked_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->button_clicked_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(action, "clicked", G_CALLBACK(Actor::_ClickActionCallback), (gpointer)obj->button_clicked_cb);

			break;

		case JSDX_TOOLKIT_EVENT_ENTER:
			if (!obj->enter_cb) {
				reg = TRUE;
				obj->enter_cb = new NodeCallback();
			} else {
				obj->enter_cb->Holder.Dispose();
				obj->enter_cb->cb.Dispose();
			}

			obj->enter_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->enter_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "enter-event", G_CALLBACK(Actor::_EnterCallback), (gpointer)obj->enter_cb);

			break;

		case JSDX_TOOLKIT_EVENT_LEAVE:
			if (!obj->leave_cb) {
				reg = TRUE;
				obj->leave_cb = new NodeCallback();
			} else {
				obj->leave_cb->Holder.Dispose();
				obj->leave_cb->cb.Dispose();
			}

			obj->leave_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->leave_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "leave-event", G_CALLBACK(Actor::_LeaveCallback), (gpointer)obj->leave_cb);

			break;

		case JSDX_TOOLKIT_EVENT_MOTION:
			if (!obj->motion_cb) {
				reg = TRUE;
				obj->motion_cb = new NodeCallback();
			} else {
				obj->motion_cb->Holder.Dispose();
				obj->motion_cb->cb.Dispose();
			}

			obj->motion_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->motion_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "motion-event", G_CALLBACK(Actor::_MotionCallback), (gpointer)obj->motion_cb);

			break;

		case JSDX_TOOLKIT_EVENT_DRAG: {
			gint x_threshold = 0;
			gint y_threshold = 0;
			ClutterDragAxis axis = CLUTTER_DRAG_AXIS_NONE;

			/* Get x_threshold, y_threshold, axis */
			if (args.Length() == 3 && Options->IsObject()) {
				if (Options->ToObject()->Get(String::New("x_threshold"))->IsNumber()) {
					x_threshold = Options->ToObject()->Get(String::New("x_threshold"))->ToInteger()->Value();
				}

				if (Options->ToObject()->Get(String::New("y_threshold"))->IsNumber()) {
					y_threshold = Options->ToObject()->Get(String::New("y_threshold"))->ToInteger()->Value();
				}

				if (Options->ToObject()->Get(String::New("axis"))->IsNumber()) {
					axis = (ClutterDragAxis)Options->ToObject()->Get(String::New("axis"))->ToInteger()->Value();
				}
			}

			/* Create action */
			action = clutter_drag_action_new();
			clutter_drag_action_set_drag_threshold(CLUTTER_DRAG_ACTION(action), x_threshold, y_threshold);
			clutter_drag_action_set_drag_axis(CLUTTER_DRAG_ACTION(action), axis);
			clutter_actor_add_action(instance, action);

			if (!obj->drag_cb) {
				reg = TRUE;
				obj->drag_cb = new NodeCallback();
			} else {
				obj->drag_cb->Holder.Dispose();
				obj->drag_cb->cb.Dispose();
			}

			obj->drag_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->drag_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg) {
				g_signal_connect(G_OBJECT(action), "drag-begin", G_CALLBACK(Actor::_DragActionBeginCallback), (gpointer)obj->drag_cb);
				g_signal_connect(G_OBJECT(action), "drag-end", G_CALLBACK(Actor::_DragActionEndCallback), (gpointer)obj->drag_cb);
				g_signal_connect(G_OBJECT(action), "drag-motion", G_CALLBACK(Actor::_DragActionMotionCallback), (gpointer)obj->drag_cb);
			}

			break;
		}
		case JSDX_TOOLKIT_EVENT_SCROLL:

			if (!obj->scroll_cb) {
				reg = TRUE;
				obj->scroll_cb = new NodeCallback();
			} else {
				obj->scroll_cb->Holder.Dispose();
				obj->scroll_cb->cb.Dispose();
			}

			obj->scroll_cb->Holder = Persistent<Object>::New(args.Holder());
			obj->scroll_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			if (reg)
				g_signal_connect(G_OBJECT(instance), "scroll-event", G_CALLBACK(Actor::_ScrollCallback), (gpointer)obj->scroll_cb);

			break;

		}

		return args.This();
	}

	Handle<Value> Actor::Off(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		/* TODO: remove event handler */

		return args.This();
	}

	/* Effect */
	Handle<Value> Actor::Effect(const Arguments &args)
	{
		HandleScope scope;
		ClutterColor color;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (args[0]->IsNumber()) {
			switch(args[0]->ToInteger()->Value()) {
			case JSDX_TOOLKIT_EFFECT_CUSTOM:
				/* TODO: Apply custom effect */
				break;

			case JSDX_TOOLKIT_EFFECT_BLUR:
				clutter_actor_add_effect(instance, clutter_blur_effect_new());
				break;

			case JSDX_TOOLKIT_EFFECT_COLORIZE:
				if (args[1]->IsObject()) {
					color.red = args[1]->ToObject()->Get(0)->ToInteger()->Value();
					color.green = args[1]->ToObject()->Get(1)->ToInteger()->Value();
					color.blue = args[1]->ToObject()->Get(2)->ToInteger()->Value();
					color.alpha = args[1]->ToObject()->Get(3)->ToInteger()->Value();

					clutter_actor_add_effect(instance, clutter_colorize_effect_new(&color));
				}
				break;

			case JSDX_TOOLKIT_EFFECT_DESATURATE:
				if (args[1]->IsNumber()) {
					clutter_actor_add_effect(instance, clutter_desaturate_effect_new(args[1]->NumberValue()));
				}
				break;

			case JSDX_TOOLKIT_EFFECT_PAGE_TURN:
				if (args[1]->IsObject()) {
					gdouble period = (gdouble)args[1]->ToObject()->Get(String::New("period"))->NumberValue();
					gdouble angle = (gdouble)args[1]->ToObject()->Get(String::New("angle"))->NumberValue();
					gfloat radius = (gfloat)args[1]->ToObject()->Get(String::New("radius"))->NumberValue();

					clutter_actor_add_effect(instance, clutter_page_turn_effect_new(period, angle, radius));
				}
				break;
			}
		}

		return args.This();
	}

	/* Animate */
	void Actor::_AnimationCompletedCallback(ClutterAnimation *animation, gpointer user_data)
	{
		NodeCallback *cb = (NodeCallback *)user_data;

		cb->cb->Call(cb->Holder, 0, NULL);

		g_signal_handlers_disconnect_by_data(G_OBJECT(animation), user_data);

		delete cb;
	}

	Handle<Value> Actor::Animate(const Arguments &args)
	{
		HandleScope scope;
		NodeCallback *cb = NULL;
		GValue value = {0};
		ClutterTimeline *timeline;
		ClutterAnimation *animation;
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (!args[0]->IsNumber())
			return ThrowException(Exception::TypeError(
				String::New("first argument must be integer")));

		if (!args[1]->IsNumber())
			return ThrowException(Exception::TypeError(
				String::New("second argument must be integer")));

		if (!args[2]->IsObject())
			return ThrowException(Exception::TypeError(
				String::New("third argument must be object")));

		if (args.Length() > 3) {
			if (args[args.Length() - 1]->IsFunction()) {

				cb = new NodeCallback();
				cb->Holder = Persistent<Object>::New(args.Holder());
				cb->cb = Persistent<Function>::New(Handle<Function>::Cast(args[args.Length() - 1]));
			}
		}

		Local<Object> properties = args[2]->ToObject();
		Local<Array> names = properties->GetOwnPropertyNames();

		if (names->Length() > 0) {
			/* Stop current animation */
			animation = ObjectWrap::Unwrap<Actor>(args.This())->_animation;
			if (animation) {
				timeline = clutter_animation_get_timeline(animation);
				if (timeline)
					clutter_timeline_pause(timeline);

				clutter_actor_detach_animation(instance);

				/* Release animation */
				ObjectWrap::Unwrap<Actor>(args.This())->_animation = NULL;
				g_object_unref(animation);
			}

			/* Create animation */
			animation = clutter_animation_new();
			ObjectWrap::Unwrap<Actor>(args.This())->_animation = animation;
			clutter_animation_set_object(animation, G_OBJECT(instance));
			clutter_animation_set_mode(animation, args[0]->ToInteger()->Value());
			clutter_animation_set_duration(animation, args[1]->ToInteger()->Value());

			/* Callback function */
			if (cb)
				g_signal_connect_after(G_OBJECT(animation), "completed", G_CALLBACK(_AnimationCompletedCallback), cb);

			/* Options */
			if (args[3]->IsObject()) {
				Local<Value> OptionValue;

				/* Loop */
				OptionValue = args[3]->ToObject()->Get(String::New("loop"));
				if (OptionValue->IsBoolean())
					clutter_timeline_set_loop(clutter_animation_get_timeline(animation), OptionValue->ToBoolean()->Value());
			}

			/* Set Properties */
			for (int i = 0; i < names->Length(); ++i) {
				Local<Value> PropertyValue = properties->Get(names->Get(i)->ToString());

				/* Prepare value */
				if (ObjectWrap::Unwrap<Actor>(args.This())->PropertyValueInit(&value, names->Get(i), PropertyValue)) {
					clutter_animation_bind(animation, *String::AsciiValue(names->Get(i)->ToString()), &value);
				}

				g_value_unset(&value);
					
			}

			/* Start Animation */
			timeline = clutter_animation_get_timeline(animation);
			if (timeline)
				clutter_timeline_start(timeline);
		}

		return args.This();
	}

	Handle<Value> Actor::SetAnimate(const Arguments &args)
	{
		HandleScope scope;
		ClutterActor *instance;
		ClutterTimeline *timeline;
		ClutterAnimation *animation;

		if (!args[0]->IsNumber())
			return ThrowException(Exception::TypeError(
				String::New("first argument must be integer")));
		
		instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
		animation = ObjectWrap::Unwrap<Actor>(args.This())->_animation;
		if (animation) {
			timeline = clutter_animation_get_timeline(animation);
			if (timeline) {
				switch(args[0]->ToInteger()->Value()) {
				case JSDX_TOOLKIT_ANIMATION_PLAY:
					clutter_timeline_start(timeline);
					break;

				case JSDX_TOOLKIT_ANIMATION_PAUSE:
					clutter_timeline_pause(timeline);
					break;

				case JSDX_TOOLKIT_ANIMATION_STOP:
					clutter_timeline_stop(timeline);
					break;

				}
			}
		}

		return args.This();
	}

	Handle<Value> Actor::ClearConstraints(const Arguments &args)
	{
		HandleScope scope;
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_clear_constraints(actor);

		return args.This();
	}

	Handle<Value> Actor::SetAlignConstraint(const Arguments &args)
	{
		HandleScope scope;
		ClutterConstraint *constraint;
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (!args[0]->IsObject())
			return ThrowException(Exception::TypeError(
				String::New("first argument must be actor")));

		if (!args[1]->IsString())
			return ThrowException(Exception::TypeError(
				String::New("second argument must be axis")));

		if (!args[2]->IsNumber())
			return ThrowException(Exception::TypeError(
				String::New("third argument must be factor")));

		ClutterActor *source = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;
		ClutterAlignAxis axis;
		gfloat factor = args[2]->ToNumber()->Value();

		if (strcmp(*String::Utf8Value(args[1]->ToString()), "x") == 0) {
			axis = CLUTTER_ALIGN_X_AXIS;
		} else if (strcmp(*String::Utf8Value(args[1]->ToString()), "y") == 0) {
			axis = CLUTTER_ALIGN_Y_AXIS;
		} else if (strcmp(*String::Utf8Value(args[1]->ToString()), "both") == 0) {
			axis = CLUTTER_ALIGN_BOTH;
		} else {
			return args.This();
		}

		constraint = clutter_align_constraint_new(source, axis, factor);
		clutter_actor_add_constraint(actor, constraint);

		return args.This();
	}

	Handle<Value> Actor::SetBindConstraint(const Arguments &args)
	{
		HandleScope scope;
		ClutterConstraint *constraint;
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (!args[0]->IsObject())
			return ThrowException(Exception::TypeError(
				String::New("first argument must be actor")));

		if (!args[1]->IsString())
			return ThrowException(Exception::TypeError(
				String::New("second argument must be coordinate")));

		if (!args[2]->IsNumber())
			return ThrowException(Exception::TypeError(
				String::New("third argument must be offset")));

		ClutterActor *source = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;
		ClutterBindCoordinate coordinate;
		gfloat offset = args[2]->ToNumber()->Value();

		if (strcmp(*String::Utf8Value(args[1]->ToString()), "x") == 0) {
			coordinate = CLUTTER_BIND_X;
		} else if (strcmp(*String::Utf8Value(args[1]->ToString()), "y") == 0) {
			coordinate = CLUTTER_BIND_Y;
		} else if (strcmp(*String::Utf8Value(args[1]->ToString()), "width") == 0) {
			coordinate = CLUTTER_BIND_WIDTH;
		} else if (strcmp(*String::Utf8Value(args[1]->ToString()), "height") == 0) {
			coordinate = CLUTTER_BIND_HEIGHT;
		} else if (strcmp(*String::Utf8Value(args[1]->ToString()), "position") == 0) {
			coordinate = CLUTTER_BIND_POSITION;
		} else if (strcmp(*String::Utf8Value(args[1]->ToString()), "size") == 0) {
			coordinate = CLUTTER_BIND_SIZE;
		} else if (strcmp(*String::Utf8Value(args[1]->ToString()), "all") == 0) {
			coordinate = CLUTTER_BIND_ALL;
		} else {
			return args.This();
		}

		constraint = clutter_bind_constraint_new(source, coordinate, offset);
		clutter_actor_add_constraint(actor, constraint);

		return args.This();
	}

}
