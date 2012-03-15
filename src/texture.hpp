#ifndef JDDX_TOOLKIT_TEXTURE_H_
#define JDDX_TOOLKIT_TEXTURE_H_

#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

	typedef enum {
		JDDX_TOOLKIT_TEXTURE_EVENT_LOAD_FINISHED = 100
	} NodeClutterTextureEvent;

	class Texture : public Actor {
	public:
		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

		v8::Persistent<v8::Function> *LoadFinishedFunc;

	protected:
		Texture();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static void _LoadFile(Texture *texture, const char *filename, bool hasCallback);
		static v8::Handle<v8::Value> LoadFile(const v8::Arguments& args);
		static v8::Handle<v8::Value> LoadFileSync(const v8::Arguments& args);

		static v8::Handle<v8::Value> FilterQualityGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void FilterQualitySetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> KeepAspectRatio(const v8::Arguments& args);
		static v8::Handle<v8::Value> On(const v8::Arguments& args);

		static void _LoadFinishedCallback(ClutterTexture *texture, GError *error, gpointer user_data);
	};

}

#endif
