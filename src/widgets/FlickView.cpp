#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../clutter.hpp"
#include "../actor.hpp"
#include "FlickView.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	FlickView::FlickView() : Actor() {
		HandleScope scope;

		/* Initializing parameters */
		ClipArea = True;
		Mode = NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_FREE_STYLE;
		Deceleration = 0.2;
		StopFactor = 100;
		Axis = NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS | NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS;
		Threshold = 100;

		/* Initializing variable */
		dx = dy = 0;
		targetDx = targetDy = 0;
		_animation = NULL;
		TotalPageX = 0;
		PageX = 0;

		/* Create FlickView */
		_actor = clutter_group_new();
		_innerBox = clutter_group_new();
		clutter_actor_set_reactive(_innerBox, TRUE);

		clutter_container_add_actor(CLUTTER_CONTAINER(_actor), _innerBox);

		/* Create action */
		_drag_action = clutter_drag_action_new();
		clutter_drag_action_set_drag_axis(CLUTTER_DRAG_ACTION(_drag_action), CLUTTER_DRAG_AXIS_NONE);
		clutter_actor_add_action(_innerBox, _drag_action);

		g_signal_connect(G_OBJECT(_drag_action), "drag-begin", G_CALLBACK(FlickView::_DragActionBeginCallback), this);
		g_signal_connect(G_OBJECT(_drag_action), "drag-end", G_CALLBACK(FlickView::_DragActionEndCallback), this);
		g_signal_connect(G_OBJECT(_drag_action), "drag-motion", G_CALLBACK(FlickView::_DragActionMotionCallback), this);

		/* Long press action */
		_long_press_action = clutter_click_action_new();
		clutter_actor_add_action(_innerBox, _long_press_action);

		g_signal_connect(_long_press_action, "long-press", G_CALLBACK(FlickView::_LongPressActionCallback), (gpointer)this);

		/* Allow user to stop animation with pressing */
		g_signal_connect(G_OBJECT(_innerBox), "button-press-event", G_CALLBACK(FlickView::_PressCallback), this);

		/* Initializing events */
		_DragBeginCallback = NULL;
		_AnimationCompletedCallback = NULL;
	}

	void FlickView::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("FlickView");

		/* Methods */
		FlickView::PrototypeMethodsInit(tpl);

		CLUTTER_DEFINE_CONSTANT(tpl, "MODE_FREE_STYLE", NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_FREE_STYLE);
		CLUTTER_DEFINE_CONSTANT(tpl, "MODE_PAGE_STYLE", NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE);

		CLUTTER_DEFINE_CONSTANT(tpl, "EVENT_DRAG_BEGIN", NODE_CLUTTER_WIDGET_FLICKVIEW_EVENT_DRAG_BEGIN);
		CLUTTER_DEFINE_CONSTANT(tpl, "EVENT_ANIMATION_COMPLETED", NODE_CLUTTER_WIDGET_FLICKVIEW_EVENT_ANIMATION_COMPLETED);

		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("clipArea"), FlickView::ClipAreaGetter, FlickView::ClipAreaSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("mode"), FlickView::ModeGetter, FlickView::ModeSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("allow_x_axis"), FlickView::AllowXAxisGetter, FlickView::AllowXAxisSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("allow_y_axis"), FlickView::AllowYAxisGetter, FlickView::AllowYAxisSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("width"), FlickView::WidthGetter, FlickView::WidthSetter);
		tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("height"), FlickView::HeightGetter, FlickView::HeightSetter);

		NODE_SET_PROTOTYPE_METHOD(tpl, "add", FlickView::Add);
		NODE_SET_PROTOTYPE_METHOD(tpl, "remove", FlickView::Remove);
		NODE_SET_PROTOTYPE_METHOD(tpl, "on", FlickView::On);

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

	Handle<Value> FlickView::ClipAreaGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		FlickView *flickview = ObjectWrap::Unwrap<FlickView>(info.This());

		return scope.Close(
			Boolean::New(flickview->ClipArea)
		);
	}

	void FlickView::ClipAreaSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			FlickView *flickview = ObjectWrap::Unwrap<FlickView>(info.This());

			flickview->ClipArea = (FlickViewMode)value->ToBoolean()->Value();

			if (flickview->ClipArea)
				clutter_actor_set_clip(CLUTTER_ACTOR(flickview->_actor),
					0, 0,
					clutter_actor_get_width(CLUTTER_ACTOR(flickview->_actor)), clutter_actor_get_height(CLUTTER_ACTOR(flickview->_actor)));
		}
	}

	Handle<Value> FlickView::ModeGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		FlickView *flickview = ObjectWrap::Unwrap<FlickView>(info.This());

		return scope.Close(
			Integer::New(flickview->Mode)
		);
	}

	void FlickView::ModeSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			FlickView *flickview = ObjectWrap::Unwrap<FlickView>(info.This());

			flickview->Mode = (FlickViewMode)value->ToUint32()->Value();
		}
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

			if (ObjectWrap::Unwrap<FlickView>(info.This())->ClipArea)
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

			if (ObjectWrap::Unwrap<FlickView>(info.This())->ClipArea)
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

	void FlickView::AnimationCompletedCallback(ClutterAnimation *animation, FlickView *flickview)
	{
		FlickView::AnimationCompletedEvent(flickview, NULL);
	}

	bool FlickView::AnimationCompleted(FlickView *flickview, float TargetX, float TargetY)
	{
		bool OutOfRange = False;
		float x = TargetX;
		float y = TargetY;

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS) {
			/* Out of range */
			if ((x > 0 && x + clutter_actor_get_width(flickview->_innerBox) >= clutter_actor_get_width(flickview->_actor))) {
				OutOfRange = True;

				if (clutter_actor_get_width(flickview->_innerBox) < clutter_actor_get_width(flickview->_actor))
					x = -clutter_actor_get_width(flickview->_innerBox) + clutter_actor_get_width(flickview->_actor);
				else
					x = 0;
					
			} else if (x < 0 && x + clutter_actor_get_width(flickview->_innerBox) < clutter_actor_get_width(flickview->_actor)) {
				OutOfRange = True;

				if (clutter_actor_get_width(flickview->_innerBox) < clutter_actor_get_width(flickview->_actor))
					x = 0;
				else
					x = -clutter_actor_get_width(flickview->_innerBox) + clutter_actor_get_width(flickview->_actor);
			}
		}

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS) {
			/* Out of range */
			if (y > 0 && y + clutter_actor_get_height(flickview->_innerBox) >= clutter_actor_get_height(flickview->_actor)) {
				OutOfRange = True;

				if (clutter_actor_get_width(flickview->_innerBox) < clutter_actor_get_width(flickview->_actor))
					y = -clutter_actor_get_height(flickview->_innerBox) + clutter_actor_get_height(flickview->_actor);
				else
					y = 0;
			} else if (y < 0 && y + clutter_actor_get_height(flickview->_innerBox) < clutter_actor_get_height(flickview->_actor)) {
				OutOfRange = True;

				if (clutter_actor_get_width(flickview->_innerBox) < clutter_actor_get_width(flickview->_actor))
					y = 0;
				else
					y = -clutter_actor_get_height(flickview->_innerBox) + clutter_actor_get_height(flickview->_actor);
			}
		}

		if (OutOfRange) {
			if (flickview->Mode == NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE)
				flickview->_animation = clutter_actor_animate(flickview->_innerBox, CLUTTER_EASE_OUT_BACK, 415,
					"x", (gfloat)x,
					"y", (gfloat)y,
					"signal-after::completed", AnimationCompletedCallback, flickview,
					NULL);
			else
				flickview->_animation = clutter_actor_animate(flickview->_innerBox, CLUTTER_EASE_OUT_SINE, 664,
					"x", (gfloat)x,
					"y", (gfloat)y,
					"signal-after::completed", AnimationCompletedCallback, flickview,
					NULL);
		}

		return OutOfRange;
	}

	void FlickView::AnimationStopCallback(ClutterAnimation *animation, FlickView *flickview)
	{
		float x, y;

		x = clutter_actor_get_x(flickview->_innerBox);
		y = clutter_actor_get_y(flickview->_innerBox);

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS)
			x += flickview->targetDx * flickview->Deceleration;

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS)
			y += flickview->targetDy * flickview->Deceleration;

		/* No Out of range */
		if (!AnimationCompleted(flickview, x, y)) {
			flickview->_animation = clutter_actor_animate(flickview->_innerBox, CLUTTER_EASE_OUT_SINE, 415,
				"x", (gfloat)x,
				"y", (gfloat)y,
				"signal-after::completed", AnimationCompletedCallback, flickview,
				NULL);
		}
	}

	int FlickView::FigurePage(FlickView *flickview)
	{
		float pos;
		int check;

		if (flickview->Mode == NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE) {
			if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS) {
				flickview->TotalPageX = ceil(clutter_actor_get_width(flickview->_innerBox) / clutter_actor_get_width(flickview->_actor));
				clutter_actor_set_width(flickview->_innerBox, flickview->TotalPageX * clutter_actor_get_width(flickview->_actor));

				/* Get current page */
				pos = fabs(clutter_actor_get_x(flickview->_innerBox)) / clutter_actor_get_width(flickview->_actor);
				check = floor(pos);

				if (pos + 1 >= check)
					flickview->PageX = check + 1;
				else
					flickview->PageX = check;
			}

			if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS) {
				flickview->TotalPageY = ceil(clutter_actor_get_height(flickview->_innerBox) / clutter_actor_get_height(flickview->_actor));
				clutter_actor_set_height(flickview->_innerBox, flickview->TotalPageY * clutter_actor_get_height(flickview->_actor));

				/* Get current page */
				pos = fabs(clutter_actor_get_y(flickview->_innerBox)) / clutter_actor_get_height(flickview->_actor);
				check = floor(pos);

				if (pos + 1 >= check)
					flickview->PageY = check + 1;
				else
					flickview->PageY = check;
			}
		}
	}

	#define FIGURE_DURATION_USEC(tv1, tv2) (((tv2).tv_usec - (tv1).tv_usec) / 1000)
	#define FIGURE_DURATION_SEC(tv1, tv2) (((tv2).tv_sec - (tv1).tv_sec))
	#define FIGURE_DURATION(tv1, tv2) (FIGURE_DURATION_SEC(tv1, tv2) * 1000 + FIGURE_DURATION_USEC(tv1, tv2))

	void FlickView::_DragActionBeginCallback(ClutterClickAction *action,
		ClutterActor *actor,
		gfloat event_x,
		gfloat event_y,
		ClutterModifierType modifiers,
		gpointer user_data)
	{
		FlickView *flickview = (FlickView *)user_data;

		FlickView::DragBeginEvent(flickview, NULL);
	}

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
		float FinalDx, FinalDy;
		bool SwitchPage = False;

		gettimeofday(&currentTime, NULL);

		/* Get currect position */
		flickview->targetX = clutter_actor_get_x(innerBox);
		flickview->targetY = clutter_actor_get_y(innerBox);

		/* Out of range right now, so do not do anything and put it back in range */
		if (AnimationCompleted(flickview, flickview->targetX, flickview->targetY))
			return;

		/* It has no action for a long time */
		if (FIGURE_DURATION(flickview->LastTimestampX, currentTime) >= flickview->StopFactor ||
			FIGURE_DURATION(flickview->LastTimestampY, currentTime) >= flickview->StopFactor) {
			flickview->dx = flickview->dy = 0;
			flickview->targetDx = flickview->targetDy = 0;

			if (flickview->Mode == NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE) {
				if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS)
					flickview->targetX = round(flickview->targetX / clutter_actor_get_width(flickview->_actor)) * clutter_actor_get_width(flickview->_actor);

				if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS)
					flickview->targetY = round(flickview->targetY / clutter_actor_get_height(flickview->_actor)) * clutter_actor_get_height(flickview->_actor);

				flickview->_animation = clutter_actor_animate(flickview->_innerBox, CLUTTER_EASE_OUT_BACK, 415,
					"x", (gfloat)flickview->targetX,
					"y", (gfloat)flickview->targetY,
					"signal-after::completed", AnimationCompletedCallback, flickview,
					NULL);
			}

			return;
		}

		/* Rate */
		if (flickview->Mode == NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE)
			DistanceRate = 1.0;
		else
			DistanceRate = 1 - flickview->Deceleration;

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_X_AXIS) {
			/* Figure duration  */
			DurationX = FIGURE_DURATION(flickview->StartTimestampX, currentTime);

			/* Accumulation */
			if (DurationX == 0)
				flickview->targetDx += flickview->dx;
			else
				flickview->targetDx += 1000 / DurationX * flickview->dx;

			FinalDx = flickview->targetDx * DistanceRate;

			/* Page style mode */
			if (flickview->Mode == NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE) {
				/* Next or previous page */
				if (fabs(FinalDx) >= flickview->Threshold) {
					flickview->PageX = flickview->PageX + ((FinalDx > 0) ? -1 : 1);

					/* Avoiding out of range */
					if (flickview->PageX <= 0)
						flickview->PageX = 1;
					else if (flickview->PageX > flickview->TotalPageX)
						flickview->PageX = flickview->TotalPageX;

					SwitchPage = True;
				}

				flickview->targetX = (flickview->PageX-1) * -clutter_actor_get_width(flickview->_actor);
			} else {
				flickview->targetX += FinalDx;
			}

		}

		if (flickview->Axis & NODE_CLUTTER_WIDGET_FLICKVIEW_Y_AXIS) {
			/* Figure duration  */
			DurationY = FIGURE_DURATION(flickview->StartTimestampY, currentTime);

			/* Accumulation */
			if (DurationY == 0)
				flickview->targetDy += flickview->dy;
			else
				flickview->targetDy += 1000 / DurationY * flickview->dy;

			FinalDy = flickview->targetDy * DistanceRate;

			/* Page style mode */
			if (flickview->Mode == NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE) {
				/* Next or previous page */
				if (fabs(FinalDy) >= flickview->Threshold) {
					flickview->PageY = flickview->PageY + ((FinalDy > 0) ? -1 : 1);

					/* Avoiding out of range */
					if (flickview->PageY <= 0)
						flickview->PageY = 1;
					else if (flickview->PageY > flickview->TotalPageY)
						flickview->PageY = flickview->TotalPageY;

					SwitchPage = True;
				}

				flickview->targetX = (flickview->PageX-1) * -clutter_actor_get_width(flickview->_actor);
			} else {
				flickview->targetY += FinalDy;
			}
		}

		/* Animation */
		if (SwitchPage)
			flickview->_animation = clutter_actor_animate(innerBox, CLUTTER_EASE_OUT_BACK, 415,
				"x", (gfloat)flickview->targetX,
				"y", (gfloat)flickview->targetY,
				"signal-after::completed", AnimationCompletedCallback, flickview,
				NULL);
		else
			flickview->_animation = clutter_actor_animate(innerBox, CLUTTER_LINEAR, 1000 * DistanceRate,
				"x", (gfloat)flickview->targetX,
				"y", (gfloat)flickview->targetY,
				"signal-after::completed", AnimationStopCallback, flickview,
				NULL);
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

		if (flickview->Mode == NODE_CLUTTER_WIDGET_FLICKVIEW_MODE_PAGE_STYLE) {
			/* Figure page number */
			FigurePage(flickview);

			/* TODO: Hide something to improve performance */
		}

		/* Reset timer */
		gettimeofday(&(flickview->StartTimestampX), NULL);
		memcpy(&flickview->StartTimestampY, &(flickview->StartTimestampX), sizeof(struct timeval));

		/* Reset delta */
		flickview->dx = flickview->dy = 0;

		return False;
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

		FlickView::AnimationCompletedEvent(flickview, NULL);

		return false;
	}

	/* Event handlers */
	void FlickView::DragBeginEvent(FlickView *flickview, gpointer userdata)
	{
		HandleScope scope;

		if (flickview->_DragBeginCallback)
			(*(flickview->_DragBeginCallback))->Call(Context::GetCurrent()->Global(), 0, NULL);
	}

	void FlickView::AnimationCompletedEvent(FlickView *flickview, gpointer userdata)
	{
		HandleScope scope;

		if (flickview->_AnimationCompletedCallback)
			(*(flickview->_AnimationCompletedCallback))->Call(Context::GetCurrent()->Global(), 0, NULL);
	}

	Handle<Value> FlickView::On(const Arguments &args)
	{
		HandleScope scope;
		Local<Value> Event;
		Local<Value> Options;
		Local<Value> Callback;
		FlickView *flickview = ObjectWrap::Unwrap<FlickView>(args.This());

		Actor::On(args);

		/* Check arguments */
		if (args.Length() == 2) {
			Event = args[0];
			Callback = args[1];
		} else if (args.Length() == 3) {
			Event = args[0];
			Options = args[1];
			Callback = args[2];
		} else
			return args.This();

		if (!Event->IsNumber()) {
			return ThrowException(Exception::TypeError(
				String::New("first argument must be integer")));
		}

		if (!Callback->IsFunction()) {
			return ThrowException(Exception::TypeError(
				String::New("Second argument must be a callback function")));
		}

		switch(Event->ToInteger()->Value()) {
		case NODE_CLUTTER_WIDGET_FLICKVIEW_EVENT_DRAG_BEGIN:
			flickview->_DragBeginCallback = new Persistent<Function>();
			*(flickview->_DragBeginCallback) = Persistent<Function>::New(Handle<Function>::Cast(Callback));
			break;

		case NODE_CLUTTER_WIDGET_FLICKVIEW_EVENT_ANIMATION_COMPLETED:
			flickview->_AnimationCompletedCallback = new Persistent<Function>();
			*(flickview->_AnimationCompletedCallback) = Persistent<Function>::New(Handle<Function>::Cast(Callback));
			break;
		}
	}

}
