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
}

void Texture::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Local<String> name = String::NewSymbol("Texture");

	/* Methods */
	PrototypeMethodsInit(tpl);

	target->Set(name, tpl->GetFunction());
}

void Texture::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
{
	HandleScope scope;

	Actor::PrototypeMethodsInit(constructor_template);

	NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Texture::LoadFile);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setLoadAsync", Texture::SetLoadAsync);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getLoadAsync", Texture::GetLoadAsync);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "keepAspectRatio", Texture::KeepAspectRatio);
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

Handle<Value> Texture::LoadFile(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsString()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_texture_set_from_file(CLUTTER_TEXTURE(instance), *String::Utf8Value(args[0]->ToString()), NULL);
		/* TODO: Need callback function for file loaded */
	}

	return args.This();
}

Handle<Value> Texture::SetLoadAsync(const Arguments &args)
{
	HandleScope scope;

	if (args[0]->IsBoolean()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_texture_set_load_async(CLUTTER_TEXTURE(instance), args[0]->ToBoolean()->Value());
	}

	return args.This();
}

Handle<Value> Texture::GetLoadAsync(const Arguments &args)
{
	HandleScope scope;

	ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

	return scope.Close(
		Boolean::New(clutter_texture_get_load_async(CLUTTER_TEXTURE(instance)))
	);
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

}
