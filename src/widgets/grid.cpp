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

#include "../clutter.hpp"
#include "../actor.hpp"
#include "../container.hpp"
#include "widget.hpp"
#include "bin.hpp"
#include "grid.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Grid::Grid() : Widget() {
		_actor = mx_grid_new();
	}

	void Grid::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Grid");

		/* Methods */
		Grid::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Grid::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		/* Inherit methods from Actor */
		Container::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("maxStride"), Grid::MaxStrideGetter, Grid::MaxStrideSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> Grid::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Grid* obj = new Grid();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> Grid::MaxStrideGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Grid>(info.This())->_actor;

		return scope.Close(
			Integer::New(mx_grid_get_max_stride(MX_GRID(instance)))
		);
	}

	void Grid::MaxStrideSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Grid>(info.This())->_actor;

			mx_grid_set_max_stride(MX_GRID(instance), value->ToInteger()->Value());
		}
	}

}
