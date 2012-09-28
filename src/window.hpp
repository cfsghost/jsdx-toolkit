#ifndef JSDX_TOOLKIT_WINDOW_H_
#define JSDX_TOOLKIT_WINDOW_H_

#include <clutter/clutter.h>
#if ENABLE_MX
#include <mx/mx.h>
#endif
#include <v8.h>
#include "stage.hpp"

namespace JSDXToolkit {

	typedef enum {
		JSDX_WINDOW_TYPE_NORMAL,
		JSDX_WINDOW_TYPE_DESKTOP,
		JSDX_WINDOW_TYPE_DOCK,
		JSDX_WINDOW_TYPE_TOOLBAR,
		JSDX_WINDOW_TYPE_MENU,
		JSDX_WINDOW_TYPE_UTILITY,
		JSDX_WINDOW_TYPE_SPLASH,
		JSDX_WINDOW_TYPE_DIALOG,
		JSDX_WINDOW_TYPE_POPUP_MENU
	} JSDXWindowType;

	class JSDXWindow : public Stage {
	public:
		static v8::Persistent<v8::FunctionTemplate> constructor;
		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);
		static v8::Local<v8::Object> New(void);

		JSDXWindow *parent;
		JSDXWindowType windowType;
		int x;
		int y;
		int width;
		int height;

#if ENABLE_MX
		MxWindow *_window;
#endif

#if USE_X11
		Window grabWindow;
		bool hasDecorator;
#endif

	protected:
		JSDXWindow();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		/* Accessor */
		static v8::Handle<v8::Value> XGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void XSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> YGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void YSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> WidthGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void WidthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> HeightGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void HeightSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> HasToolbarGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void HasToolbarSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> RotationGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void RotationSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> HasDecoratorGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void HasDecoratorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> WindowTypeGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void WindowTypeSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		/* Methods */
		static v8::Handle<v8::Value> SetChild(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetParentWindow(const v8::Arguments& args);
		static v8::Handle<v8::Value> Show(const v8::Arguments& args);
		static v8::Handle<v8::Value> ShowAll(const v8::Arguments& args);
		static v8::Handle<v8::Value> Hide(const v8::Arguments& args);

		/* signal */
		static void _HideCallback(ClutterActor *actor, gpointer user_data);
#if USE_X11
		static ClutterX11FilterReturn MenuEventHandler(XEvent *xev, ClutterEvent *cev, gpointer data);
#endif
	};

}

#endif
