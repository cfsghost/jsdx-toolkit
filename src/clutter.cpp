#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "stage.hpp"

namespace clutter {
 
using namespace node;
using namespace v8;
 
static Handle<Value> ClutterInit(const Arguments& args)
{
	HandleScope scope;

	return scope.Close(Integer::New(clutter_init(NULL, NULL)));
}

static Handle<Value> ClutterMain(const Arguments& args)
{
	clutter_main();

	return Undefined();
}

static Handle<Value> SetUseARGBVisual(const Arguments& args)
{
	if (args[0]->IsBoolean()) {
		clutter_x11_set_use_argb_visual(args[0]->ToBoolean()->Value());
	}

	return Undefined();
}
 
extern "C" {
	static void init(Handle<Object> target)
	{
		HandleScope scope;

		/* Error conditions for initializing*/
		CLUTTER_DEFINE_CONSTANT(target, "INIT_SUCCESS", CLUTTER_INIT_SUCCESS);
		CLUTTER_DEFINE_CONSTANT(target, "INIT_UNKNOWN", CLUTTER_INIT_ERROR_UNKNOWN);
		CLUTTER_DEFINE_CONSTANT(target, "INIT_THREADS", CLUTTER_INIT_ERROR_THREADS);
		CLUTTER_DEFINE_CONSTANT(target, "INIT_BACKEND", CLUTTER_INIT_ERROR_BACKEND);
		CLUTTER_DEFINE_CONSTANT(target, "INIT_INTERNAL", CLUTTER_INIT_ERROR_INTERNAL);

		NODE_SET_METHOD(target, "init", ClutterInit);
		NODE_SET_METHOD(target, "main", ClutterMain);
		NODE_SET_METHOD(target, "useARGB", SetUseARGBVisual);

		Actor::Initialize(target);
		Stage::Initialize(target);
	}

	NODE_MODULE(clutter, init);
}

}
