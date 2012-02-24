#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <string.h>

#include "clutter.hpp"
#include "actor.hpp"

namespace clutter {
 
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

Actor::Actor()
	: ObjectWrap() {}

void Actor::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	Local<String> name = String::NewSymbol("Actor");

	PrototypeMethodsInit(tpl);

	target->Set(name, tpl->GetFunction());
}

void Actor::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
{
	HandleScope scope;

	NODE_SET_PROTOTYPE_METHOD(constructor_template, "destroy", Actor::Destroy);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "show", Actor::Show);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "showAll", Actor::ShowAll);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "hide", Actor::Hide);

	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setOpacity", Actor::SetOpacity);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getOpacity", Actor::GetOpacity);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "resize", Actor::Resize);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setWidth", Actor::SetWidth);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getWidth", Actor::GetWidth);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setHeight", Actor::SetHeight);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getHeight", Actor::GetHeight);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setPosition", Actor::SetPosition);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setDepth", Actor::SetDepth);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getDepth", Actor::GetDepth);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setX", Actor::SetX);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getX", Actor::GetX);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setY", Actor::SetY);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getY", Actor::GetY);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "reactive", Actor::Reactive);

	/* Scale */
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "scale", Actor::Scale);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setScaleGravity", Actor::SetScaleGravity);

	NODE_SET_PROTOTYPE_METHOD(constructor_template, "rotate", Actor::Rotate);

	/* Event handler */
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "on", Actor::On);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "_off", Actor::Off);

	/* Effect */
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "effect", Actor::Effect);

	/* Animation */
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "animate", Actor::Animate);
}

void Actor::PropertyValueInit(GValue *gvalue, Handle<Value> property, Handle<Value> value)
{
	int i;

	for (i = 0; properties[i].name != NULL; ++i) {
		if (strcmp(properties[i].name, *String::AsciiValue(property->ToString())) == 0) {
			g_value_init(gvalue, properties[i].type);

			switch(properties[i].type) {
			case G_TYPE_UINT:
				g_value_set_uint(gvalue, value->ToInteger()->Value());
				break;

			case G_TYPE_FLOAT:
				g_value_set_float(gvalue, value->NumberValue());
				break;

			case G_TYPE_DOUBLE:
				g_value_set_double(gvalue, value->NumberValue());
				break;

			}
		}
	}
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

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
	clutter_actor_destroy(instance);

	return args.This();
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

Handle<Value> Actor::SetOpacity(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_opacity(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetOpacity(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Integer::New(clutter_actor_get_opacity(CLUTTER_ACTOR(instance)))
	);
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

Handle<Value> Actor::SetWidth(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_width(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetWidth(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Number::New(clutter_actor_get_width(CLUTTER_ACTOR(instance)))
	);
}

Handle<Value> Actor::SetHeight(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_height(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetHeight(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Number::New(clutter_actor_get_height(CLUTTER_ACTOR(instance)))
	);
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

Handle<Value> Actor::SetX(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_x(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetX(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Number::New(clutter_actor_get_x(CLUTTER_ACTOR(instance)))
	);
}

Handle<Value> Actor::SetY(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_y(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetY(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Number::New(clutter_actor_get_y(CLUTTER_ACTOR(instance)))
	);
}

Handle<Value> Actor::SetDepth(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsNumber()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_actor_set_depth(CLUTTER_ACTOR(instance), args[0]->NumberValue());
	}

	return args.This();
}

Handle<Value> Actor::GetDepth(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Number::New(clutter_actor_get_depth(CLUTTER_ACTOR(instance)))
	);
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
					args[0]->NumberValue(),
					args[1]->NumberValue(), 
					(ClutterGravity)args[2]->ToInteger()->Value());

		} else if (args.Length() == 4) {
			if (args[2]->IsNumber() && args[3]->IsNumber())
				clutter_actor_set_scale_full(CLUTTER_ACTOR(instance),
					args[0]->NumberValue(),
					args[1]->NumberValue(), 
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

Handle<Value> Actor::Reactive(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	if (args[0]->IsUndefined() || args[0]->IsNull()) {
		return scope.Close(
			Boolean::New(clutter_actor_get_reactive(CLUTTER_ACTOR(instance)))
		);
	}

	if (args[0]->IsBoolean())
		clutter_actor_set_reactive(CLUTTER_ACTOR(instance), args[0]->ToBoolean()->Value());


	return args.This();
}

/* Event Dispatcher */
void Actor::_DestroyCallback(ClutterActor *actor, gpointer user_data)
{
	Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(user_data);

	(*callback)->Call(Context::GetCurrent()->Global(), 0, NULL);
}

void Actor::_ClickActionCallback(ClutterClickAction *action, ClutterActor *actor, gpointer user_data)
{
	const unsigned argc = 1;
	Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(user_data);

	Local<Value> argv[argc] = {
		Local<Value>::New(Integer::New(NODE_CLUTTER_EVENT_CLICK))
	};

	(*callback)->Call(Context::GetCurrent()->Global(), argc, argv);
}

gboolean Actor::_EnterCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
	const unsigned argc = 1;
	Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(user_data);

	Local<Value> argv[argc] = {
		Local<Value>::New(Integer::New(NODE_CLUTTER_EVENT_ENTER))
	};

	Local<Value> ret = (*callback)->Call(Context::GetCurrent()->Global(), argc, argv);

	return (*ret)->IsTrue();
}

gboolean Actor::_LeaveCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
	const unsigned argc = 1;
	Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(user_data);

	Local<Value> argv[argc] = {
		Local<Value>::New(Integer::New(NODE_CLUTTER_EVENT_LEAVE))
	};

	Local<Value> ret = (*callback)->Call(Context::GetCurrent()->Global(), argc, argv);

	return (*ret)->IsTrue();
}

gboolean Actor::_MotionCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
	const unsigned argc = 2;
	Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(user_data);

	/* create a JavaScript Object */
	Local<Object> o = Object::New();
	o->Set(String::New("x"), Number::New(event->motion.x));
	o->Set(String::New("y"), Number::New(event->motion.y));
	o->Set(String::New("time"), Uint32::New(event->motion.time));

	Local<Value> argv[argc] = {
		Local<Value>::New(Integer::New(NODE_CLUTTER_EVENT_MOTION)),
		o
	};

	Local<Value> ret = (*callback)->Call(Context::GetCurrent()->Global(), argc, argv);

	return (*ret)->IsTrue();
}

Handle<Value> Actor::On(const Arguments &args)
{
	HandleScope scope;
	Actor *obj = ObjectWrap::Unwrap<Actor>(args.This());

	ClutterAction *action;
	ClutterActor *instance = obj->_actor;

	/* Check arguments */
	if (args.Length() != 2)
		return args.This();

	if (!args[0]->IsNumber()) {
		return ThrowException(Exception::TypeError(
			String::New("first argument must be integer")));
	}

	if (!args[1]->IsFunction()) {
		return ThrowException(Exception::TypeError(
			String::New("Second argument must be a callback function")));
    }

	/* Get callback function */
	Persistent<Function> *callback = new Persistent<Function>();
	*callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));

	switch(args[0]->ToInteger()->Value()) {
	case NODE_CLUTTER_EVENT_DESTROY:
		g_signal_connect(G_OBJECT(instance), "destroy", G_CALLBACK(Actor::_DestroyCallback), (gpointer)callback);

		break;

	case NODE_CLUTTER_EVENT_CLICK:
		action = clutter_click_action_new();
		clutter_actor_add_action(instance, action);

		g_signal_connect(action, "clicked", G_CALLBACK(Actor::_ClickActionCallback), (gpointer)callback);

		break;

	case NODE_CLUTTER_EVENT_ENTER:
		g_signal_connect(G_OBJECT(instance), "enter-event", G_CALLBACK(Actor::_EnterCallback), (gpointer)callback);

		break;

	case NODE_CLUTTER_EVENT_LEAVE:
		g_signal_connect(G_OBJECT(instance), "leave-event", G_CALLBACK(Actor::_LeaveCallback), (gpointer)callback);

		break;

	case NODE_CLUTTER_EVENT_MOTION:
		g_signal_connect(G_OBJECT(instance), "motion-event", G_CALLBACK(Actor::_MotionCallback), (gpointer)callback);

		break;
	}

	return args.This();
}

Handle<Value> Actor::Off(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

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
		case NODE_CLUTTER_EFFECT_CUSTOM:
			/* TODO: Apply custom effect */
			break;

		case NODE_CLUTTER_EFFECT_BLUR:
			clutter_actor_add_effect(instance, clutter_blur_effect_new());
			break;

		case NODE_CLUTTER_EFFECT_COLORIZE:
			if (args[1]->IsObject()) {
				color.red = args[1]->ToObject()->Get(0)->ToInteger()->Value();
				color.green = args[1]->ToObject()->Get(1)->ToInteger()->Value();
				color.blue = args[1]->ToObject()->Get(2)->ToInteger()->Value();
				color.alpha = args[1]->ToObject()->Get(3)->ToInteger()->Value();

				clutter_actor_add_effect(instance, clutter_colorize_effect_new(&color));
			}
			break;

		case NODE_CLUTTER_EFFECT_DESATURATE:
			if (args[1]->IsNumber()) {
				clutter_actor_add_effect(instance, clutter_desaturate_effect_new(args[1]->NumberValue()));
			}
			break;

		case NODE_CLUTTER_EFFECT_PAGE_TURN:
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
Handle<Value> Actor::Animate(const Arguments &args)
{
	HandleScope scope;
	ClutterAnimation *animation;
	GValue value = {0};

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsObject()) {
		Local<Object> properties = args[2]->ToObject();
		Local<Array> names = properties->GetOwnPropertyNames();

		/* Create animation */
		animation = clutter_animation_new();
		clutter_animation_set_object(animation, G_OBJECT(instance));
		clutter_animation_set_mode(animation, args[0]->ToInteger()->Value());
		clutter_animation_set_duration(animation, args[1]->ToInteger()->Value());

		/* Set Properties */
		for (int i = 0; i < names->Length(); ++i) {
			Local<Value> property = properties->Get(names->Get(i)->ToString());

			/* Prepare value */
			ObjectWrap::Unwrap<Actor>(args.This())->PropertyValueInit(&value, names->Get(i), property);

			clutter_animation_bind(animation, *String::AsciiValue(names->Get(i)->ToString()), &value);

			g_value_unset(&value);
		}

		/* Loop */
		if (args[3]->IsBoolean()) {
			clutter_timeline_set_loop(clutter_animation_get_timeline(animation), args[3]->ToBoolean()->Value());
		}

		/* Start Animation */
		clutter_timeline_start(clutter_animation_get_timeline(animation));
	}

	return args.This();
}

}
