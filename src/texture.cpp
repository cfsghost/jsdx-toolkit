/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#if USE_X11
#include <clutter/x11/clutter-x11.h>
#endif

#include "jsdx_toolkit.hpp"
#include "actor.hpp"
#include "texture.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Texture::Texture() : Actor() {
		HandleScope scope;

		_actor = NULL;
		load_finished_cb = NULL;
	}

	Texture::~Texture() {
		delete load_finished_cb;
	}

	void Texture::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Texture");

		JSDX_TOOLKIT_DEFINE_CONSTANT(tpl, "QUALITY_LOW", CLUTTER_TEXTURE_QUALITY_LOW);
		JSDX_TOOLKIT_DEFINE_CONSTANT(tpl, "QUALITY_MEDIUM", CLUTTER_TEXTURE_QUALITY_MEDIUM);
		JSDX_TOOLKIT_DEFINE_CONSTANT(tpl, "QUALITY_HIGH", CLUTTER_TEXTURE_QUALITY_HIGH);

		/* Event */
		JSDX_TOOLKIT_DEFINE_CONSTANT(tpl, "EVENT_LOAD_FINISHED", JSDX_TOOLKIT_TEXTURE_EVENT_LOAD_FINISHED);

		/* Methods */
		PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Texture::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Actor::PrototypeMethodsInit(constructor_template);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("filterQuality"), Texture::FilterQualityGetter, Texture::FilterQualitySetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("repeatX"), Texture::RepeatXGetter, Texture::RepeatXSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("repeatY"), Texture::RepeatYGetter, Texture::RepeatYSetter);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Texture::LoadFile);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFileSync", Texture::LoadFileSync);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "keepAspectRatio", Texture::KeepAspectRatio);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "on", Texture::On);

#if USE_X11
		/* Sync X11 window */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setX11Window", Texture::SetX11Window);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setX11WindowAutoSync", Texture::SetX11WindowAutoSync);
#endif
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
#if USE_X11
		if (args[0]->IsNumber()) {
			if (args[0]->ToInteger()->Value() == JSDX_TOOLKIT_TEXTURE_X11) {
				obj->_actor = clutter_x11_texture_pixmap_new();

				return scope.Close(args.This());
			}
		}
#endif
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
				if (!texture->load_finished_cb) {
					texture->load_finished_cb = new NodeCallback();
				} else {
					texture->load_finished_cb->Holder.Dispose();
					texture->load_finished_cb->cb.Dispose();
				}

				texture->load_finished_cb->Holder = Persistent<Object>::New(args.Holder());
				texture->load_finished_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(args[1]));

				Texture::_LoadFile(texture, *String::Utf8Value(args[0]->ToString()), TRUE);
			} else {
				Texture::_LoadFile(texture, *String::Utf8Value(args[0]->ToString()), FALSE);
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

	Handle<Value> Texture::RepeatXGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;
		gboolean x, y;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		clutter_texture_get_repeat(CLUTTER_TEXTURE(instance), &x, &y);

		return scope.Close(
			Boolean::New(x)
		);
	}

	void Texture::RepeatXSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			g_object_set(G_OBJECT(instance), "repeat-x", value->ToBoolean()->Value(), NULL);
		}
	}

	Handle<Value> Texture::RepeatYGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;
		gboolean x, y;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		clutter_texture_get_repeat(CLUTTER_TEXTURE(instance), &x, &y);

		return scope.Close(
			Boolean::New(y)
		);
	}

	void Texture::RepeatYSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			g_object_set(G_OBJECT(instance), "repeat-y", value->ToBoolean()->Value(), NULL);
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

#if USE_X11
	Handle<Value> Texture::SetX11Window(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (args[0]->IsNumber()) {
			XWindowAttributes xwa;

			/* Ping this window to make sure it's live */
			if (XGetWindowAttributes(clutter_x11_get_default_display(), (Window)(args[0]->ToInteger()->Value()), &xwa)) {
				clutter_x11_texture_pixmap_set_window((ClutterX11TexturePixmap *)instance, (Window)(args[0]->ToInteger()->Value()), FALSE);
			}
		}

		return args.This();
	}

	Handle<Value> Texture::SetX11WindowAutoSync(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		if (args[0]->IsBoolean()) {
			clutter_x11_texture_pixmap_sync_window((ClutterX11TexturePixmap *)instance);
			clutter_x11_texture_pixmap_set_automatic((ClutterX11TexturePixmap *)instance, args[0]->ToBoolean()->Value());
		}

		return args.This();
	}
#endif

	/* Event handlers */
	void Texture::_LoadFinishedCallback(ClutterTexture *tex, GError *error, gpointer user_data)
	{
		Texture *texture = (Texture *)user_data;
		texture->load_finished_cb->cb->Call(texture->load_finished_cb->Holder, 0, NULL);

		delete texture->load_finished_cb;

		texture->load_finished_cb = NULL;
	}

	Handle<Value> Texture::On(const Arguments &args)
	{
		HandleScope scope;
		Local<Value> Event;
		Local<Value> Options;
		Local<Value> Callback;
		ClutterActor *instance = ObjectWrap::Unwrap<Texture>(args.This())->_actor;
#if 0
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
#endif
		Actor::On(args);

		return args.This();
	}

}
