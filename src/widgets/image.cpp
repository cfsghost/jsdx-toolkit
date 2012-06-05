/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <mx/mx.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../jsdx_toolkit.hpp"
#include "../actor.hpp"
#include "../container.hpp"
#include "widget.hpp"
#include "image.hpp"
#include "stylable.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Image::Image() : Widget() {
		_actor = mx_image_new();
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
		Stylable::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("scaleMode"), Image::ScaleModeGetter, Image::ScaleModeSetter);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Image::LoadFile);
	}

	/* ECMAScript constructor */
	Handle<Value> Image::New(const Arguments& args)
	{
		HandleScope scope;

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

	/* Accessor */
	Handle<Value> Image::ScaleModeGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Image>(info.This())->_actor;

		return scope.Close(
			Integer::New(mx_image_get_scale_mode(MX_IMAGE(instance)))
		);
	}

	void Image::ScaleModeSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Image>(info.This())->_actor;

			mx_image_set_scale_mode(MX_IMAGE(instance), (MxImageScaleMode)value->ToInteger()->Value());
		}
	}

	/* Methods */
	void Image::_LoadFile(Image *image, const char *filename, NodeCallback *imgcb)
	{
		CoglHandle cogltex;
		MxImage *instance = MX_IMAGE(image->_actor);

		if (imgcb) {
			g_signal_connect(G_OBJECT(instance), "image-loaded", G_CALLBACK(Image::_ImageLoadedCallback), (gpointer)imgcb);
		}

		mx_image_set_load_async(MX_IMAGE(instance), TRUE);
		mx_image_set_scale_mode(MX_IMAGE(instance), MX_IMAGE_SCALE_FIT);
		mx_image_set_from_file(MX_IMAGE(instance), filename, NULL);
	}

	Handle<Value> Image::LoadFile(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			Image *image = ObjectWrap::Unwrap<Image>(args.This());

			if (args[1]->IsFunction()) {
				/* Create a callback object */
				NodeCallback *imgcb = new NodeCallback();
				imgcb->Holder = Persistent<Object>::New(args.Holder());
				imgcb->cb = Persistent<Function>::New(Handle<Function>::Cast(args[1]));

				_LoadFile(image, *String::Utf8Value(args[0]->ToString()), imgcb);

			} else {
				_LoadFile(image, *String::Utf8Value(args[0]->ToString()), NULL);
			}
		}

		return args.This();
	}

	/* Event Handlers */
	void Image::_ImageLoadedCallback(MxImage *img, gpointer user_data)
	{
		NodeCallback *cb = (NodeCallback *)user_data;

		TryCatch try_catch;
		cb->cb->Call(cb->Holder, 0, NULL);
		if (try_catch.HasCaught()) {
			node::FatalException(try_catch);
		}

		delete cb;
	}

}
