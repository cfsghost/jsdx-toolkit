#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "media.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

Media::Media() : Actor() {}

void Media::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Local<String> name = String::NewSymbol("Media");

	/* Methods */
	Actor::PrototypeMethodsInit(tpl);
	NODE_SET_PROTOTYPE_METHOD(tpl, "loadFile", Media::LoadFile);
	NODE_SET_PROTOTYPE_METHOD(tpl, "loadFileURI", Media::LoadFileURI);
	NODE_SET_PROTOTYPE_METHOD(tpl, "play", Media::Play);
	NODE_SET_PROTOTYPE_METHOD(tpl, "pause", Media::Pause);
	NODE_SET_PROTOTYPE_METHOD(tpl, "setVolume", Media::SetVolume);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getVolume", Media::GetVolume);

	target->Set(name, tpl->GetFunction());
}

/* ECMAScript constructor */
Handle<Value> Media::New(const Arguments& args)
{
	HandleScope scope;
	static ClutterColor color;

	if (!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of this object."))
		);
	}

	// Creates a new instance object of this type and wraps it.
	Media* obj = new Media();
	obj->Wrap(args.This());

	return scope.Close(args.This());
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

}
