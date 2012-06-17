#ifndef JSDX_TOOLKIT_APPLICATION_H_
#define JSDX_TOOLKIT_APPLICATION_H_

#include <clutter/clutter.h>
#if ENABLE_MX
#include <mx/mx.h>
#endif
#if USE_X11
#include <clutter/x11/clutter-x11.h>
#endif
#include <v8.h>

namespace JSDXToolkit {

	class Application : public node::ObjectWrap {
	public:
		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

#if ENABLE_MX
		MxApplication *_application;
#endif

	protected:
		Application(const char *name);

		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		/* Accessors */
		static v8::Handle<v8::Value> IsRunningGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void IsRunningSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		/* Methods */
		static v8::Handle<v8::Value> Run(const v8::Arguments &args);
		static v8::Handle<v8::Value> Quit(const v8::Arguments &args);
		static v8::Handle<v8::Value> Add(const v8::Arguments &args);
		static v8::Handle<v8::Value> LoadStyleFile(const v8::Arguments &args);
	};

}

#endif
