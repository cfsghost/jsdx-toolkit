#ifndef JDDX_TOOLKIT_MEDIA_H_
#define JDDX_TOOLKIT_MEDIA_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

	class Media : node::ObjectWrap {
	public:
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

	protected:
		Media();

		static v8::Handle<v8::Value> LoadFile(const v8::Arguments& args);
		static v8::Handle<v8::Value> LoadFileURI(const v8::Arguments& args);
		static v8::Handle<v8::Value> Play(const v8::Arguments& args);
		static v8::Handle<v8::Value> Pause(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetVolume(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetVolume(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetProgress(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetProgress(const v8::Arguments& args);
	};

}

#endif
