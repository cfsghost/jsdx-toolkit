#ifndef NODE_CLUTTER_WIDGETS_FLICKVIEW_H_
#define NODE_CLUTTER_WIDGETS_FLICKVIEW_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>
#include <sys/time.h>

namespace clutter {

class FlickView : public Actor {
public:
	static void Initialize(v8::Handle<v8::Object> target);

	int StopFactor;
	float Deceleration;
	struct timeval StartTimestampX;
	struct timeval StartTimestampY;
	struct timeval LastTimestampX;
	struct timeval LastTimestampY;
	double dx;
	double dy;
	double targetDx;
	double targetDy;

	ClutterActor *_innerBox;
	ClutterAction *_drag_action;
	ClutterAction *_long_press_action;
	ClutterAnimation *_animation;

protected:
	FlickView();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> Add(const v8::Arguments& args);
	static v8::Handle<v8::Value> Remove(const v8::Arguments& args);

	static void _DragActionEndCallback(ClutterClickAction *action,
		ClutterActor *actor,
		gfloat event_x,
		gfloat event_y,
		ClutterModifierType modifiers,
		gpointer user_data);
	static void _DragActionMotionCallback(ClutterClickAction *action,
		ClutterActor *actor,
		gfloat delta_x,
		gfloat delta_y,
		gpointer user_data);

	static gboolean _PressCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
	static gboolean _LongPressActionCallback(ClutterClickAction *action, ClutterActor *actor, ClutterLongPressState state, gpointer user_data);
};

}

#endif
