#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "container.hpp"
#include "stage.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> Stage::constructor;

	Stage::Stage() : Container() {
		HandleScope scope;

		/* Create Stage */
		_actor = clutter_stage_new();

		/* TODO: Binding destroy event */
	}

	void Stage::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Stage");

		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		/* Methods */
		Stage::PrototypeMethodsInit(constructor);

		target->Set(name, constructor->GetFunction());
	}

	void Stage::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Container::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("title"), Stage::TitleGetter, Stage::TitleSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("useFog"), Stage::UseFogGetter, Stage::UseFogSetter);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "useAlpha", Stage::SetUseAlpha);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setColor", Stage::SetColor);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setCursor", Stage::SetCursor);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "getFog", Stage::GetFog);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setFog", Stage::SetFog);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "fullscreen", Stage::Fullscreen);
	}

	Local<Object> Stage::New(void)
	{
		Local<Object> ObjectInstance = Stage::constructor->GetFunction()->NewInstance();
		Stage* obj = new Stage();
		obj->Wrap(ObjectInstance);

		return ObjectInstance;
	}

	/* ECMAScript constructor */
	Handle<Value> Stage::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Stage* obj = new Stage();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> Stage::TitleGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			String::New(clutter_stage_get_title(CLUTTER_STAGE(instance)))
		);
	}

	void Stage::TitleSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_stage_set_title(CLUTTER_STAGE(instance), *String::Utf8Value(value->ToString()));
		}
	}

	Handle<Value> Stage::UseFogGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Boolean::New(clutter_stage_get_use_fog(CLUTTER_STAGE(instance)))
		);
	}

	void Stage::UseFogSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_stage_set_use_fog(CLUTTER_STAGE(instance), value->ToBoolean()->Value());
		}
	}

	Handle<Value> Stage::GetFog(const Arguments &args)
	{
		HandleScope scope;
		ClutterFog fog;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_stage_get_fog(CLUTTER_STAGE(instance), &fog);

		/* create a JavaScript Object */
		Local<Object> o = Object::New();
		o->Set(String::New("z_near"), Number::New(fog.z_near));
		o->Set(String::New("z_far"), Number::New(fog.z_far));

		return scope.Close(
			o
		);
	}

	Handle<Value> Stage::SetFog(const Arguments &args)
	{
		HandleScope scope;
		ClutterFog fog;

		if (args[0]->IsNumber() && args[1]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			fog.z_near = args[0]->NumberValue();
			fog.z_far = args[1]->NumberValue();

			clutter_stage_set_fog(CLUTTER_STAGE(instance), &fog);
		}
	}

	Handle<Value> Stage::SetUseAlpha(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_stage_set_use_alpha(CLUTTER_STAGE(instance), args[0]->ToBoolean()->Value());
		}

		return args.This();
	}

	Handle<Value> Stage::SetColor(const Arguments &args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			color.red = args[0]->NumberValue();
			color.green = args[1]->NumberValue();
			color.blue = args[2]->NumberValue();
			color.alpha = args[3]->NumberValue();

			clutter_stage_set_color(CLUTTER_STAGE(instance), &color);
		}

		return args.This();
	}

	Handle<Value> Stage::SetCursor(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			if (args[0]->ToBoolean()->Value())
				clutter_stage_show_cursor(CLUTTER_STAGE(instance));
			else
				clutter_stage_hide_cursor(CLUTTER_STAGE(instance));
		}

		return args.This();
	}

	Handle<Value> Stage::Fullscreen(const Arguments &args)
	{
		HandleScope scope;
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (args[0]->IsUndefined() || args[0]->IsNull()) {
			return scope.Close(
				Boolean::New(clutter_stage_get_fullscreen(CLUTTER_STAGE(instance)))
			);
		}

		if (args[0]->IsBoolean()) {
			clutter_stage_set_fullscreen(CLUTTER_STAGE(instance), args[0]->ToBoolean()->Value());
		}

		return args.This();
	}

}
