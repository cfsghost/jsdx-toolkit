#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "media.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

Media::Media() : node::ObjectWrap() {}

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

}
