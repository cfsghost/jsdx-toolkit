#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <sys/time.h>
#include <string.h>

#include "../clutter.hpp"
#include "../actor.hpp"
#include "FlickView.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	FlickView::FlickView() : Actor() {
		HandleScope scope;

		/* Initializing parameters */
		Deceleration = 0.2;
		StopFactor = 100;
		Axis = NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS | NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS;

		/* Initializing variable */
		dx = dy = 0;
		targetDx = targetDy = 0;
		_animation = NULL;

		/* Create FlickView */
		_actor = clutter_group_new();
		_innerBox = clutter_group_new();
		clutter_actor_set_reactive(_innerBox, TRUE);

		clutter_container_add_actor(CLUTTER_CONTAINER(_actor), _innerBox);

		/* Create action */
		_drag_action = clutter_drag_action_new();
		clutter_drag_action_set_drag_axis(CLUTTER_DRAG_ACTION(_drag_action), CLUTTER_DRAG_AXIS_NONE);
		clutter_actor_add_action(_innerBox, _drag_action);

		g_signal_connect(G_OBJECT(_drag_action), "drag-end", G_CALLBACK(FlickView::_DragActionEndCallback), this);
		g_signal_connect(G_OBJECT(_drag_action), "drag-motion", G_CALLBACK(FlickView::_DragActionMotionCallback), this);

		/* Long press action */
		_long_press_action = clutter_click_action_new();
		clutter_actor_add_action(_innerBox, _long_press_action);

		g_signal_connect(_long_press_action, "long-press", G_CALLBACK(FlickView::_LongPressActionCallback), (gpointer)this);

		/* Allow user to stop animation with pressing */
		g_signal_connect(G_OBJECT(_innerBox), "button-press-event", G_CALLBACK(FlickView::_PressCallback), this);
	}

	void FlickView::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("FlickView");

		/* Methods */
		FlickView::PrototypeMethodsInit(tpl);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("allow_x_axis"), FlickView::AllowXAxisGetter, FlickView::AllowXAxisSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("allow_y_axis"), FlickView::AllowYAxisGetter, FlickView::AllowYAxisSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("width"), FlickView::WidthGetter, FlickView::WidthSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("height"), FlickView::HeightGetter, FlickView::HeightSetter);

		NODE_SET_PROTOTYPE_METHOD(tpl, "add", FlickView::Add);
		NODE_SET_PROTOTYPE_METHOD(tpl, "remove", FlickView::Remove);

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> FlickView::New(const Arguments& args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		FlickView* obj = new FlickView();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> FlickView::Add(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<FlickView>(args.This())->_innerBox;
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

		clutter_container_add_actor(CLUTTER_CONTAINER(instance), CLUTTER_ACTOR(actor));

		return args.This();
	}

	Handle<Value> FlickView::Remove(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<FlickView>(args.This())->_innerBox;
		ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

		clutter_container_remove_actor(CLUTTER_CONTAINER(instance), CLUTTER_ACTOR(actor));

		return args.This();
	}

	Handle<Value> FlickView::WidthGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Number::New(clutter_actor_get_width(CLUTTER_ACTOR(instance)))
		);
	}

	void FlickView::WidthSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_width(CLUTTER_ACTOR(instance), value->NumberValue());

			clutter_actor_set_clip(CLUTTER_ACTOR(instance), 0, 0, value->NumberValue(), clutter_actor_get_height(CLUTTER_ACTOR(instance)));
		}
	}

	Handle<Value> FlickView::HeightGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Number::New(clutter_actor_get_height(CLUTTER_ACTOR(instance)))
		);
	}

	void FlickView::HeightSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

			clutter_actor_set_height(CLUTTER_ACTOR(instance), value->NumberValue());

			clutter_actor_set_clip(CLUTTER_ACTOR(instance), 0, 0, clutter_actor_get_width(CLUTTER_ACTOR(instance)), value->NumberValue());
		}
	}

	Handle<Value> FlickView::AllowXAxisGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		FlickView *flickview = ObjectWrap::Unwrap<FlickView>(info.This());


		return scope.Close(
			Boolean::New((flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS) ? True : False)
		);
	}

	void FlickView::AllowXAxisSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			FlickView *flickview = ObjectWrap::Unwrap<FlickView>(info.This());

			if (value->ToBoolean()->Value()) {
				flickview->Axis |= NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS;
			} else {
				flickview->Axis &= ~NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS;

				clutter_drag_action_set_drag_axis(CLUTTER_DRAG_ACTION(flickview->_drag_action), CLUTTER_DRAG_Y_AXIS);
			}
		}
	}

	Handle<Value> FlickView::AllowYAxisGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		FlickView *flickview = ObjectWrap::Unwrap<FlickView>(info.This());


		return scope.Close(
			Boolean::New((flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS) ? True : False)
		);
	}

	void FlickView::AllowYAxisSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			FlickView *flickview = ObjectWrap::Unwrap<FlickView>(info.This());

			if (value->ToBoolean()->Value()) {
				flickview->Axis |= NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS;
			} else {
				flickview->Axis &= ~NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS;

				clutter_drag_action_set_drag_axis(CLUTTER_DRAG_ACTION(flickview->_drag_action), CLUTTER_DRAG_X_AXIS);
			}
		}
	}

	void FlickView::AnimationStopCallback(ClutterAnimation *animation, FlickView *flickview)
	{
		float x, y;

		x = clutter_actor_get_x(flickview->_innerBox);
		y = clutter_actor_get_y(flickview->_innerBox);

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS) {
			x += flickview->targetDx * flickview->Deceleration;

			if ((x > 0 && x + clutter_actor_get_width(flickview->_innerBox) > clutter_actor_get_width(flickview->_actor)))
				x = -clutter_actor_get_width(flickview->_innerBox) + clutter_actor_get_width(flickview->_actor);
			else if (x < 0 && x + clutter_actor_get_width(flickview->_innerBox) < clutter_actor_get_width(flickview->_actor))
				x = 0;
		}

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS) {
			y += flickview->targetDy * flickview->Deceleration;

			if (y > 0 && y + clutter_actor_get_height(flickview->_innerBox) > clutter_actor_get_height(flickview->_actor))
				y = -clutter_actor_get_height(flickview->_innerBox) + clutter_actor_get_height(flickview->_actor);
			else if (y < 0 && y + clutter_actor_get_height(flickview->_innerBox) < clutter_actor_get_height(flickview->_actor))
				y = 0;
		}

		flickview->_animation = clutter_actor_animate(flickview->_innerBox, CLUTTER_EASE_OUT_QUAD, 1000,
			"x", x,
			"y", y,
			NULL);
		
	}

	#define FIGURE_DURATION_USEC(tv1, tv2) (((tv2).tv_usec - (tv1).tv_usec) / 1000)
	#define FIGURE_DURATION_SEC(tv1, tv2) (((tv2).tv_sec - (tv1).tv_sec))
	#define FIGURE_DURATION(tv1, tv2) (FIGURE_DURATION_SEC(tv1, tv2) * 1000 + FIGURE_DURATION_USEC(tv1, tv2))

	void FlickView::_DragActionEndCallback(ClutterClickAction *action,
		ClutterActor *actor,
		gfloat event_x,
		gfloat event_y,
		ClutterModifierType modifiers,
		gpointer user_data)
	{
		FlickView *flickview = (FlickView *)user_data;
		ClutterActor *innerBox = flickview->_innerBox;
		struct timeval currentTime;
		long DurationX;
		long DurationY;
		float DistanceRate;

		/* Get currect position */
		flickview->targetX = clutter_actor_get_x(innerBox);
		flickview->targetY = clutter_actor_get_y(innerBox);

		gettimeofday(&currentTime, NULL);

		/* It has no action for a long time */
		if (FIGURE_DURATION(flickview->LastTimestampX, currentTime) >= flickview->StopFactor ||
			FIGURE_DURATION(flickview->LastTimestampY, currentTime) >= flickview->StopFactor) {
			flickview->dx = flickview->dy = 0;
			flickview->targetDx = flickview->targetDy = 0;
		}

		/* Rate */
		DistanceRate = 1 - flickview->Deceleration;

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS) {
			/* Figure duration  */
			DurationX = FIGURE_DURATION(flickview->StartTimestampX, currentTime);

			/* Accumulation */
			if (DurationX == 0)
				flickview->targetDx += flickview->dx;
			else
				flickview->targetDx += 1000 / DurationX * flickview->dx;

			flickview->targetX += flickview->targetDx * DistanceRate;
		}

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS) {
			/* Figure duration  */
			DurationY = FIGURE_DURATION(flickview->StartTimestampY, currentTime);

			/* Accumulation */
			if (DurationY == 0)
				flickview->targetDy += flickview->dy;
			else
				flickview->targetDy += 1000 / DurationY * flickview->dy;

			flickview->targetY += flickview->targetDy * DistanceRate;
		}

		/* Animation */
		flickview->_animation = clutter_actor_animate(innerBox, CLUTTER_LINEAR, 1000 * DistanceRate,
			"x", flickview->targetX,
			"y", flickview->targetY,
			"signal-after::completed", AnimationStopCallback, flickview,
			NULL);
/*
			"x", clutter_actor_get_x(innerBox) + flickview->targetDx * flickview->Deceleration,
			"y", clutter_actor_get_y(innerBox) + flickview->targetDy * flickview->Deceleration,
*/
	}

	void FlickView::_DragActionMotionCallback(ClutterClickAction *action,
		ClutterActor *actor,
		gfloat delta_x,
		gfloat delta_y,
		gpointer user_data)
	{
		FlickView *flickview = (FlickView *)user_data;

		/* Get last timestamp with action */
		gettimeofday(&(flickview->LastTimestampX), NULL);
		memcpy(&flickview->LastTimestampY, &(flickview->LastTimestampX), sizeof(struct timeval));

		/* Reset delta and timer if they have different sign */
		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS) {
			if (((flickview->dx >= 0 && delta_x >= 0) || (flickview->dx <= 0 && delta_x <= 0)) &&
				((flickview->targetDx >= 0 && delta_x >= 0) || (flickview->targetDx <= 0 && delta_x <= 0))) {
				flickview->dx += delta_x;
			} else {
				flickview->dx = delta_x;
				memcpy(&flickview->StartTimestampX, &(flickview->LastTimestampX), sizeof(struct timeval));
				flickview->targetDx = flickview->targetDy = 0;
			}
		}

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS) {
			if (((flickview->dy >= 0 && delta_y >= 0) || (flickview->dy <= 0 && delta_y <= 0)) &&
				((flickview->targetDy >= 0 && delta_y >= 0) || (flickview->targetDy <= 0 && delta_y <= 0))) {
				flickview->dy += delta_y;
			} else {
				flickview->dy = delta_y;
				memcpy(&flickview->StartTimestampY, &(flickview->LastTimestampY), sizeof(struct timeval));
				flickview->targetDx = flickview->targetDy = 0;
			}
		}
	}

	gboolean FlickView::_PressCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
	{
		FlickView *flickview = (FlickView *)user_data;
		ClutterTimeline *timeline;

		/* Pause animation right now */
		if (clutter_actor_get_animation(flickview->_innerBox) != NULL) {
			timeline = clutter_animation_get_timeline(flickview->_animation);
			if (timeline)
				clutter_timeline_pause(timeline);
		}

		/* Reset timer */
		gettimeofday(&(flickview->StartTimestampX), NULL);
		memcpy(&flickview->StartTimestampY, &(flickview->StartTimestampX), sizeof(struct timeval));

		/* Reset delta */
		flickview->dx = flickview->dy = 0;
	}

	gboolean FlickView::_LongPressActionCallback(ClutterClickAction *action, ClutterActor *actor, ClutterLongPressState state, gpointer user_data)
	{
		FlickView *flickview = (FlickView *)user_data;

		switch(state) {
		case CLUTTER_LONG_PRESS_QUERY:
			return true;
		case CLUTTER_LONG_PRESS_ACTIVATE:
			flickview->targetDx = flickview->targetDy = 0;
			return true;
		}

		return false;
	}
}
