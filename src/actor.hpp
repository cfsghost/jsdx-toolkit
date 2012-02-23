#ifndef NODE_CLUTTER_ACTOR_H_
#define NODE_CLUTTER_ACTOR_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class Actor : public node::ObjectWrap {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

	ClutterActor *_actor;

protected:
	Actor();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> Destroy(const v8::Arguments& args);
	static v8::Handle<v8::Value> Show(const v8::Arguments& args);
	static v8::Handle<v8::Value> ShowAll(const v8::Arguments& args);
	static v8::Handle<v8::Value> Hide(const v8::Arguments& args);

	static v8::Handle<v8::Value> SetOpacity(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetOpacity(const v8::Arguments& args);
	static v8::Handle<v8::Value> Resize(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetWidth(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetWidth(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetHeight(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetHeight(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetPosition(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetDepth(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetDepth(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetX(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetX(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetY(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetY(const v8::Arguments& args);
	static v8::Handle<v8::Value> Reactive(const v8::Arguments& args);

	static v8::Handle<v8::Value> Scale(const v8::Arguments& args);
	static v8::Handle<v8::Value> Rotate(const v8::Arguments& args);

	static v8::Handle<v8::Value> Animate(const v8::Arguments& args);

	static v8::Handle<v8::Value> On(const v8::Arguments& args);
	static v8::Handle<v8::Value> Off(const v8::Arguments& args);

	static void _DestroyCallback(ClutterActor *actor, gpointer user_data);
	static void _ClickActionCallback(ClutterClickAction *action, ClutterActor *actor, gpointer user_data);
	static gboolean _EnterCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
	static gboolean _LeaveCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
	static gboolean _MotionCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
};

}

#endif
