#ifndef JSDX_TOOLKIT_MEDIA_H_
#define JSDX_TOOLKIT_MEDIA_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

	class Media : public Texture {
	public:
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

		NodeCallback *signal_eos_cb;
		NodeCallback *signal_error_cb;

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

		static v8::Handle<v8::Value> On(const v8::Arguments& args);

		/* Signal callback */
		static void _SignalEOSCallback(ClutterMedia *media, gpointer user_data);
		static void _SignalErrorCallback(ClutterMedia *media, GError *error, gpointer user_data);
	};

}

#endif
