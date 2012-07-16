#ifndef JSDX_TOOLKIT_ACTOR_H_
#define JSDX_TOOLKIT_ACTOR_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

	typedef enum {
		JSDX_TOOLKIT_ANIMATION_PLAY,
		JSDX_TOOLKIT_ANIMATION_PAUSE,
		JSDX_TOOLKIT_ANIMATION_STOP
	} NodeClutterAnimationCommand;

	struct PropertyDefine {
		const char *name;
		GType type;
	};

	struct EventDefine {
		int id;
		const char *event;
	};

	class Actor : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> constructor_template;

		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

		bool PropertyValueInit(GValue *gvalue, v8::Handle<v8::Value> property, v8::Handle<v8::Value> value);

		ClutterActor *_actor;
		ClutterAnimation *_animation;

		NodeCallback *destroy_cb;
		NodeCallback *button_press_cb;
		NodeCallback *button_release_cb;
		NodeCallback *button_clicked_cb;
		NodeCallback *key_press_cb;
		NodeCallback *key_release_cb;
		NodeCallback *long_press_cb;
		NodeCallback *enter_cb;
		NodeCallback *leave_cb;
		NodeCallback *motion_cb;
		NodeCallback *drag_cb;
		NodeCallback *scroll_cb;

	protected:
		Actor();
		~Actor();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> Destroy(const v8::Arguments& args);
		static v8::Handle<v8::Value> Show(const v8::Arguments& args);
		static v8::Handle<v8::Value> ShowAll(const v8::Arguments& args);
		static v8::Handle<v8::Value> Hide(const v8::Arguments& args);

		static v8::Handle<v8::Value> XGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> YGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> DepthGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void XSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void YSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void DepthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> OpacityGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void OpacitySetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> WidthGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void WidthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> HeightGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void HeightSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> HasClipGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void HasClipSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> ReactiveGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void ReactiveSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> MarginTopGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void MarginTopSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> Resize(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetPosition(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetBackgroundColor(const v8::Arguments& args);
		static v8::Handle<v8::Value> RaiseTop(const v8::Arguments& args);
		static v8::Handle<v8::Value> LowerBottom(const v8::Arguments& args);
		static v8::Handle<v8::Value> IsVisible(const v8::Arguments& args);

		static v8::Handle<v8::Value> SetAnchorFromGravity(const v8::Arguments& args);

		static v8::Handle<v8::Value> Scale(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetScaleGravity(const v8::Arguments& args);

		static v8::Handle<v8::Value> Rotate(const v8::Arguments& args);

		static v8::Handle<v8::Value> Effect(const v8::Arguments& args);
		static v8::Handle<v8::Value> Animate(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetAnimate(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetAlignConstraint(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetBindConstraint(const v8::Arguments& args);
		static void _AnimationCompletedCallback(ClutterAnimation *animation, gpointer user_data);

		static v8::Handle<v8::Value> On(const v8::Arguments& args);
		static v8::Handle<v8::Value> Off(const v8::Arguments& args);

		static void _DestroyCallback(ClutterActor *actor, gpointer user_data);
		static void _ClickActionCallback(ClutterClickAction *action, ClutterActor *actor, gpointer user_data);
		static gboolean _LongPressActionCallback(ClutterClickAction *action, ClutterActor *actor, ClutterLongPressState state, gpointer user_data);
		static gboolean _PressCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
		static gboolean _ReleaseCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
		static gboolean _KeyPressCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
		static gboolean _KeyReleaseCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
		static gboolean _EnterCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
		static gboolean _LeaveCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
		static gboolean _MotionCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);

		static void _DragActionBeginCallback(ClutterClickAction *action,
			ClutterActor *actor,
			gfloat event_x,
			gfloat event_y,
			ClutterModifierType modifiers,
			gpointer user_data);
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

		static gboolean _ScrollCallback(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
	};

}

#endif
