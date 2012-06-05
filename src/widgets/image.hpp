#ifndef JSDX_TOOLKIT_WIDGET_IMAGE_H_
#define JSDX_TOOLKIT_WIDGET_IMAGE_H_

#include <clutter/clutter.h>
#include <mx/mx.h>
#include <v8.h>
#include "widget.hpp"

namespace JSDXToolkit {

struct ImageCallback;

class Image : public Widget {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Image();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	/* Accessor */
	static v8::Handle<v8::Value> ScaleModeGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void ScaleModeSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	//static void _LoadFile(Image *image, const char *filename, bool hasCallback);
	static void _LoadFile(Image *image, const char *filename, NodeCallback *imgcb);
	static v8::Handle<v8::Value> LoadFile(const v8::Arguments& args);

	static void _ImageLoadedCallback(MxImage *img, gpointer user_data);
};

}

#endif
