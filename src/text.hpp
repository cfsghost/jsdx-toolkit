#ifndef JSDX_TOOLKIT_TEXT_H_
#define JSDX_TOOLKIT_TEXT_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

	class Text : public Actor {
	public:
		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

	protected:
		Text();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		/* Accessor */
		static v8::Handle<v8::Value> TextGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void TextSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> EditableGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void EditableSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		/* Methods */
		static v8::Handle<v8::Value> Focus(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetColor(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetColor(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetFontName(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetFontName(const v8::Arguments& args);
	};

}

#endif
