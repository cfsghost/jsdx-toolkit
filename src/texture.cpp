#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "texture.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Texture::Texture() : Actor() {
		HandleScope scope;

		_actor = NULL;
		LoadFinishedFunc = new Persistent<Function>();
	}

	void Texture::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Texture");

		CLUTTER_DEFINE_CONSTANT(tpl, "QUALITY_LOW", CLUTTER_TEXTURE_QUALITY_LOW);
		CLUTTER_DEFINE_CONSTANT(tpl, "QUALITY_MEDIUM", CLUTTER_TEXTURE_QUALITY_MEDIUM);
		CLUTTER_DEFINE_CONSTANT(tpl, "QUALITY_HIGH", CLUTTER_TEXTURE_QUALITY_HIGH);

		/* Event */
		CLUTTER_DEFINE_CONSTANT(tpl, "EVENT_LOAD_FINISHED", NODE_CLUTTER_TEXTURE_EVENT_LOAD_FINISHED);

		/* Methods */
		PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Texture::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Actor::PrototypeMethodsInit(constructor_template);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("filterQuality"), Texture::FilterQualityGetter, Texture::FilterQualitySetter);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Texture::LoadFile);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFileSync", Texture::LoadFileSync);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "keepAspectRatio", Texture::KeepAspectRatio);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "on", Texture::On);
	}

	/* ECMAScript constructor */
	Handle<Value> Texture::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Texture* obj = new Texture();
		obj->Wrap(args.This());

		/* Create Texture */
		obj->_actor = clutter_texture_new();

		return scope.Close(args.This());
	}

	void Texture::_LoadFile(Texture *texture, const char *filename, bool hasCallback)
	{
		CoglHandle cogltex;
		ClutterActor *instance = texture->_actor;

		if (hasCallback)
			g_signal_connect(G_OBJECT(instance), "load-finished", G_CALLBACK(Texture::_LoadFinishedCallback), (gpointer)texture);

		clutter_texture_set_load_async(CLUTTER_TEXTURE(instance), TRUE);

		clutter_texture_set_from_file(CLUTTER_TEXTURE(instance), filename, NULL);
	}

	Handle<Value> Texture::LoadFile(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			Texture *texture = ObjectWrap::Unwrap<Texture>(args.This());

			if (args[1]->IsFunction()) {
				*(texture->LoadFinishedFunc) = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
				_LoadFile(texture, *String::Utf8Value(args[0]->ToString()), True);
			} else {
				_LoadFile(texture, *String::Utf8Value(args[0]->ToString()), False);
			}
		}

		return args.This();
	}

	Handle<Value> Texture::LoadFileSync(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			Actor *actor = ObjectWrap::Unwrap<Actor>(args.This());
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
			clutter_texture_set_load_async(CLUTTER_TEXTURE(instance), FALSE);
			
			return scope.Close(
				Boolean::New(clutter_texture_set_from_file(CLUTTER_TEXTURE(instance), *String::Utf8Value(args[0]->ToString()), NULL))
			);

			/* TODO: Need callback function for file loaded */
		}

		return scope.Close(
			Boolean::New(False)
		);
	}

	Handle<Value> Texture::FilterQualityGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Integer::New(clutter_texture_get_filter_quality(CLUTTER_TEXTURE(instance)))
		);
	}

	void Texture::FilterQualitySetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_texture_set_filter_quality(CLUTTER_TEXTURE(instance), (ClutterTextureQuality)value->ToInteger()->Value());
		}
	}

	Handle<Value> Texture::KeepAspectRatio(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		/* Return value if no parameter */
		if (args[0]->IsUndefined() || args[0]->IsNull())
			return scope.Close(
				Boolean::New(clutter_texture_get_keep_aspect_ratio(CLUTTER_TEXTURE(instance)))
			);

		if (args[0]->IsBoolean()) {
			clutter_texture_set_keep_aspect_ratio(CLUTTER_TEXTURE(instance), args[0]->ToBoolean()->Value());
		}

		return args.This();
	}

	/* Event handlers */
	void Texture::_LoadFinishedCallback(ClutterTexture *tex, GError *error, gpointer user_data)
	{
		Texture *texture = (Texture *)user_data;
		Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(texture->LoadFinishedFunc);

		(*callback)->Call(Context::GetCurrent()->Global(), 0, 0);
	}

	Handle<Value> Texture::On(const Arguments &args)
	{
		HandleScope scope;
		Local<Value> Event;
		Local<Value> Options;
		Local<Value> Callback;
		ClutterActor *instance = ObjectWrap::Unwrap<Texture>(args.This())->_actor;

		Actor::On(args);

		/* Check arguments */
		if (args.Length() == 2) {
			Event = args[0];
			Callback = args[1];
		} else if (args.Length() == 3) {
			Event = args[0];
			Options = args[1];
			Callback = args[2];
		} else
			return args.This();

		if (!Event->IsNumber()) {
			return ThrowException(Exception::TypeError(
				String::New("first argument must be integer")));
		}

		if (!Callback->IsFunction()) {
			return ThrowException(Exception::TypeError(
				String::New("Second argument must be a callback function")));
		}

		/* Get callback function */
		Persistent<Function> *callback = new Persistent<Function>();
		*callback = Persistent<Function>::New(Handle<Function>::Cast(Callback));

		switch(Event->ToInteger()->Value()) {
		case NODE_CLUTTER_TEXTURE_EVENT_LOAD_FINISHED:
	//		g_signal_connect(G_OBJECT(instance), "load-finished", G_CALLBACK(Texture::_LoadFinishedCallback), (gpointer)callback);
			break;
		}
	}

}
