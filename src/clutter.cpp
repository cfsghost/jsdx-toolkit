#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>

#include "clutter.hpp"
#include "actor.hpp"
#include "container.hpp"
#include "group.hpp"
#include "stage.hpp"
#include "rectangle.hpp"
#include "text.hpp"
#include "texture.hpp"

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

static Handle<Value> ClutterMainQuit(const Arguments& args)
{
	clutter_main_quit();

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

		/* ClutterGravity */
		CLUTTER_DEFINE_CONSTANT(target, "GRAVITY_NONE", CLUTTER_GRAVITY_NONE);
		CLUTTER_DEFINE_CONSTANT(target, "GRAVITY_NORTH", CLUTTER_GRAVITY_NORTH);
		CLUTTER_DEFINE_CONSTANT(target, "GRAVITY_NORTH_EAST", CLUTTER_GRAVITY_NORTH_EAST);
		CLUTTER_DEFINE_CONSTANT(target, "GRAVITY_EAST", CLUTTER_GRAVITY_EAST);
		CLUTTER_DEFINE_CONSTANT(target, "GRAVITY_SOUTH_EAST", CLUTTER_GRAVITY_SOUTH_EAST);
		CLUTTER_DEFINE_CONSTANT(target, "GRAVITY_SOUTH", CLUTTER_GRAVITY_SOUTH);
		CLUTTER_DEFINE_CONSTANT(target, "GRAVITY_SOUTH_WEST", CLUTTER_GRAVITY_SOUTH_WEST);
		CLUTTER_DEFINE_CONSTANT(target, "GRAVITY_CENTER", CLUTTER_GRAVITY_CENTER);

		/* ClutterRotateAxis */
		CLUTTER_DEFINE_CONSTANT(target, "X_AXIS", CLUTTER_X_AXIS);
		CLUTTER_DEFINE_CONSTANT(target, "Y_AXIS", CLUTTER_Y_AXIS);
		CLUTTER_DEFINE_CONSTANT(target, "Z_AXIS", CLUTTER_Z_AXIS);

		/* ClutterRotateDirection */
		CLUTTER_DEFINE_CONSTANT(target, "ROTATE_CW", CLUTTER_ROTATE_CW);
		CLUTTER_DEFINE_CONSTANT(target, "ROTATE_CCW", CLUTTER_ROTATE_CCW);

		/* own events */
		CLUTTER_DEFINE_CONSTANT(target, "EVENT_CLICK", NODE_CLUTTER_EVENT_CLICK);
		CLUTTER_DEFINE_CONSTANT(target, "EVENT_LONG_PRESS", NODE_CLUTTER_EVENT_LONG_PRESS);

		NODE_SET_METHOD(target, "init", ClutterInit);
		NODE_SET_METHOD(target, "main", ClutterMain);
		NODE_SET_METHOD(target, "quit", ClutterMainQuit);
		NODE_SET_METHOD(target, "useARGB", SetUseARGBVisual);

		Actor::Initialize(target);
		Container::Initialize(target);
		Group::Initialize(target);
		Stage::Initialize(target);
		Rectangle::Initialize(target);
		Text::Initialize(target);
		Texture::Initialize(target);
	}

	NODE_MODULE(clutter, init);
}

}
