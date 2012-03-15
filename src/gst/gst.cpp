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

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Gst::Gst() : Texture() {}

	void Gst::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		/* Add gst_init() to module */
		NODE_SET_METHOD(target, "gst_init", Gst::Init);
	}

	void Gst::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Texture::PrototypeMethodsInit(constructor_template);
		Media::PrototypeMethodsInit(constructor_template);
	}

	Handle<Value> Gst::Init(const Arguments& args)
	{
		HandleScope scope;

		return scope.Close(Integer::New(clutter_gst_init(NULL, NULL)));
	}

}
