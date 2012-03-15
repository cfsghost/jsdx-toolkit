/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <clutter-gst/clutter-gst.h>

#include "../clutter.hpp"
#include "../actor.hpp"

#include "gst.hpp"
#include "video-texture.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	GstVideoTexture::GstVideoTexture() : Gst() {
		HandleScope scope;

		_actor = clutter_gst_video_texture_new();
	}

	void GstVideoTexture::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("GstVideoTexture");

		/* Methods */
		Gst::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	/* ECMAScript constructor */
	Handle<Value> GstVideoTexture::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		GstVideoTexture* obj = new GstVideoTexture();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

}
