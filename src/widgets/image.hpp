#ifndef NODE_CLUTTER_WIDGET_IMAGE_H_
#define NODE_CLUTTER_WIDGET_IMAGE_H_

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

	v8::Persistent<v8::Function> *ImageLoadedFunc;

protected:
	Image();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	//static void _LoadFile(Image *image, const char *filename, bool hasCallback);
	static void _LoadFile(Image *image, const char *filename, ImageCallback *imgcb);
	static v8::Handle<v8::Value> LoadFile(const v8::Arguments& args);

	static void _ImageLoadedCallback(MxImage *img, gpointer user_data);
};

struct ImageCallback {
	Image *object;
	v8::Persistent<v8::Function> callback;
};

}

#endif
