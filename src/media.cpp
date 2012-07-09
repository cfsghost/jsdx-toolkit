/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <string.h>
#include <clutter/clutter.h>

#include "jsdx_toolkit.hpp"
#include "actor.hpp"
#include "texture.hpp"
#include "media.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Media::Media() : Texture() {

		signal_eos_cb = NULL;
		signal_error_cb = NULL;
	}

	void Media::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Media::LoadFile);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFileURI", Media::LoadFileURI);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "play", Media::Play);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "pause", Media::Pause);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setVolume", Media::SetVolume);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "getVolume", Media::GetVolume);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "setProgress", Media::SetProgress);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "getProgress", Media::GetProgress);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "on", Media::On);
	}

	Handle<Value> Media::LoadFile(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_media_set_filename(CLUTTER_MEDIA(instance), *String::Utf8Value(args[0]->ToString()));
		}

		return args.This();
	}

	Handle<Value> Media::LoadFileURI(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_media_set_uri(CLUTTER_MEDIA(instance), *String::Utf8Value(args[0]->ToString()));
		}

		return args.This();
	}

	Handle<Value> Media::Play(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_media_set_playing(CLUTTER_MEDIA(instance), TRUE);

		return args.This();
	}

	Handle<Value> Media::Pause(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_media_set_playing(CLUTTER_MEDIA(instance), FALSE);

		return args.This();
	}

	Handle<Value> Media::SetVolume(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_media_set_audio_volume(CLUTTER_MEDIA(instance), args[0]->NumberValue());
		}

		return args.This();
	}

	Handle<Value> Media::GetVolume(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		return scope.Close(
			Number::New(clutter_media_get_audio_volume(CLUTTER_MEDIA(instance)))
		);
	}

	Handle<Value> Media::SetProgress(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

			clutter_media_set_progress(CLUTTER_MEDIA(instance), args[0]->NumberValue());
		}

		return args.This();
	}

	Handle<Value> Media::GetProgress(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		return scope.Close(
			Number::New(clutter_media_get_progress(CLUTTER_MEDIA(instance)))
		);
	}

	Handle<Value> Media::On(const Arguments &args)
	{
		HandleScope scope;
		Local<Value> Event;
		Local<Value> Options;
		Local<Value> Callback;

		Media *media = ObjectWrap::Unwrap<Media>(args.This());
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

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

		if (strcmp(*String::Utf8Value(Event->ToString()), "eos") == 0) {

			if (!media->signal_eos_cb) {
				media->signal_eos_cb = new NodeCallback;
			} else {
				media->signal_eos_cb->Holder.Dispose();
				media->signal_eos_cb->cb.Dispose();
			}

			media->signal_eos_cb->Holder = Persistent<Object>::New(args.Holder());
			media->signal_eos_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			g_signal_connect(G_OBJECT(instance), "eos", G_CALLBACK(Media::_SignalEOSCallback), (gpointer)media->signal_eos_cb);

		} else if (strcmp(*String::Utf8Value(Event->ToString()), "err") == 0) {

			if (!media->signal_error_cb) {
				media->signal_error_cb = new NodeCallback;
			} else {
				media->signal_error_cb->Holder.Dispose();
				media->signal_error_cb->cb.Dispose();
			}

			media->signal_error_cb->Holder = Persistent<Object>::New(args.Holder());
			media->signal_error_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(Callback));

			g_signal_connect(G_OBJECT(instance), "error", G_CALLBACK(Media::_SignalErrorCallback), (gpointer)media->signal_error_cb);
		} else {
			Texture::On(args);
		}

		return args.This();
	}

	/* Signal callback */
	void Media::_SignalEOSCallback(ClutterMedia *media, gpointer user_data)
	{
		NodeCallback *cb = (NodeCallback *)user_data;

		cb->cb->Call(cb->Holder, 0, 0);
	}

	void Media::_SignalErrorCallback(ClutterMedia *media, GError *error, gpointer user_data)
	{
		NodeCallback *cb = (NodeCallback *)user_data;

		cb->cb->Call(cb->Holder, 0, 0);
	}
}
