#ifndef JSDX_TOOLKIT_WINDOW_H_
#define JSDX_TOOLKIT_WINDOW_H_

#include <clutter/clutter.h>
#if ENABLE_MX
#include <mx/mx.h>
#endif
#include <v8.h>
#include "stage.hpp"

namespace JSDXToolkit {

	class JSDXWindow : public Stage {
	public:
		static v8::Persistent<v8::FunctionTemplate> constructor;
		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);
		static v8::Local<v8::Object> New(void);

#if ENABLE_MX
		MxWindow *_window;
#endif

#if USE_X11
		bool hasDecorator;
#endif

	protected:
		JSDXWindow();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		/* Accessor */
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

		/* Methods */
		static v8::Handle<v8::Value> SetChild(const v8::Arguments& args);
		static v8::Handle<v8::Value> Show(const v8::Arguments& args);
		static v8::Handle<v8::Value> ShowAll(const v8::Arguments& args);
	};

}

#endif
