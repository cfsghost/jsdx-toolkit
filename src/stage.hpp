#ifndef JSDX_TOOLKIT_STAGE_H_
#define JSDX_TOOLKIT_STAGE_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>
#include "actor.hpp"

namespace JSDXToolkit {

	class Stage : public Actor {
	public:
		static v8::Persistent<v8::FunctionTemplate> constructor;
		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);
		static v8::Local<v8::Object> New(void);

	protected:
		Stage();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetUseAlpha(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetColor(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetCursor(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetFog(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetFog(const v8::Arguments& args);
		static v8::Handle<v8::Value> Fullscreen(const v8::Arguments& args);

		static v8::Handle<v8::Value> TitleGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void TitleSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> UseFogGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void UseFogSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	};

}

#endif
