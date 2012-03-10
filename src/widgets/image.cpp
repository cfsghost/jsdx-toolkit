#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <mx/mx.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../clutter.hpp"
#include "../actor.hpp"
#include "../container.hpp"
#include "widget.hpp"
#include "image.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Image::Image() : Widget() {
		_actor = mx_image_new();

		ImageLoadedFunc = new Persistent<Function>();
	}

	void Image::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Image");

		/* Methods */
		Image::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Image::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Image::LoadFile);
	}

	/* ECMAScript constructor */
	Handle<Value> Image::New(const Arguments& args)
	{
		HandleScope scope;
		static ClutterColor color;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Image* obj = new Image();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	void Image::_LoadFile(Image *image, const char *filename, bool hasCallback)
	{
		CoglHandle cogltex;
		MxImage *instance = MX_IMAGE(image->_actor);

		if (hasCallback)
			g_signal_connect(G_OBJECT(instance), "image-loaded", G_CALLBACK(Image::_ImageLoadedCallback), (gpointer)image);

		mx_image_set_load_async(MX_IMAGE(instance), TRUE);

		mx_image_set_from_file(MX_IMAGE(instance), filename, NULL);
	}

	Handle<Value> Image::LoadFile(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			Image *image = ObjectWrap::Unwrap<Image>(args.This());

			if (args[1]->IsFunction()) {
				*(image->ImageLoadedFunc) = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
				_LoadFile(image, *String::Utf8Value(args[0]->ToString()), True);

			} else {
				_LoadFile(image, *String::Utf8Value(args[0]->ToString()), False);
			}
		}

		return args.This();
	}

	/* Event Handlers */
	void Image::_ImageLoadedCallback(MxImage *img, gpointer user_data)
	{
		Image *image = (Image *)user_data;
		Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(image->ImageLoadedFunc);

		(*callback)->Call(Context::GetCurrent()->Global(), 0, 0);
	}

}
