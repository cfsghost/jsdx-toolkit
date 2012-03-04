#ifndef NODE_CLUTTER_WIDGETS_FLICKVIEW_H_
#define NODE_CLUTTER_WIDGETS_FLICKVIEW_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>
#include <sys/time.h>

namespace clutter {

typedef enum {
	NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS = (1 << 0),
	NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS = (1 << 1)
} FlickViewAxis;

typedef enum {
	NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_FREE_STYLE,
	NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE
} FlickViewMode;

class FlickView : public Actor {
public:
	static void Initialize(v8::Handle<v8::Object> target);

	FlickViewMode Mode;
	int Axis;
	int StopFactor;
	float Deceleration;
	float Threshold;

	int TotalPageX;
	int TotalPageY;
	int PageX;
	int PageY;
	struct timeval StartTimestampX;
	struct timeval StartTimestampY;
	struct timeval LastTimestampX;
	struct timeval LastTimestampY;
	double dx;
	double dy;
	double targetDx;
	double targetDy;
	double targetX;
	double targetY;

	ClutterActor *_innerBox;
	ClutterAction *_drag_action;
	ClutterAction *_long_press_action;
	ClutterAnimation *_animation;

protected:
	FlickView();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> Add(const v8::Arguments& args);
	static v8::Handle<v8::Value> Remove(const v8::Arguments& args);

	static v8::Handle<v8::Value> ModeGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void ModeSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> AllowXAxisGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static v8::Handle<v8::Value> AllowYAxisGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void AllowXAxisSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	static void AllowYAxisSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> WidthGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static v8::Handle<v8::Value> HeightGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void WidthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	static void HeightSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static int FigurePage(FlickView *flickview);
	static bool AnimationCompleted(FlickView *flickview, float TargetX, float TargetY);
	static void AnimationStopCallback(ClutterAnimation *animation, FlickView *flickview);
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
