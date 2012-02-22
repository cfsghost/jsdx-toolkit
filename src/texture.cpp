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

	/* Create Texture */
	_actor = clutter_texture_new();
}

void Texture::Initialize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Local<String> name = String::NewSymbol("Texture");

	/* Methods */
	Actor::PrototypeMethodsInit(tpl);
	NODE_SET_PROTOTYPE_METHOD(tpl, "loadFile", Texture::LoadFile);

	target->Set(name, tpl->GetFunction());
}

/* ECMAScript constructor */
Handle<Value> Texture::New(const Arguments& args)
{
	HandleScope scope;
	static ClutterColor color;

	if (!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of this object."))
		);
	}

	// Creates a new instance object of this type and wraps it.
	Texture* obj = new Texture();
	obj->Wrap(args.This());

	return scope.Close(args.This());
}

Handle<Value> Texture::LoadFile(const Arguments &args)
{
	HandleScope scope;
	static ClutterColor color;

	if (args[0]->IsString()) {
		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		clutter_texture_set_from_file(CLUTTER_TEXTURE(instance), *String::Utf8Value(args[0]->ToString()), NULL);
		/* TODO: Need callback function for file loaded */
	}

	return args.This();
}

}
