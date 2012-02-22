#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "stage.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;

Stage::Stage() : Actor() {
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

	Actor::PrototypeMethodsInit(tpl);

	target->Set(name, tpl->GetFunction());
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

}
