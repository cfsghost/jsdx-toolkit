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
#include "bin.hpp"
#include "table.hpp"

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> Table::constructor;

	Table::Table() : Widget() {
		_actor = mx_table_new();
	}

	void Table::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		Local<String> name = String::NewSymbol("Table");

		/* Constructor */
		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		/* Methods */
		Table::PrototypeMethodsInit(constructor);

		target->Set(name, constructor->GetFunction());
	}

	void Table::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		/* Inherit methods from Actor */
		Container::PrototypeMethodsInit(constructor_template);

		/* Accessor */
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("columnSpacing"), Table::ColumnSpacingGetter, Table::ColumnSpacingSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("rowSpacing"), Table::RowSpacingGetter, Table::RowSpacingSetter);

		/* Methods */
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "_add", Table::Add);
	}

	/* ECMAScript constructor */
	Handle<Value> Table::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Table* obj = new Table();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	/* Accessor */
	Handle<Value> Table::ColumnSpacingGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Table>(info.This())->_actor;

		return scope.Close(
			Integer::New(mx_table_get_column_spacing(MX_TABLE(instance)))
		);
	}

	void Table::ColumnSpacingSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Table>(info.This())->_actor;

			mx_table_set_column_spacing(MX_TABLE(instance), value->ToInteger()->Value());
		}
	}

	Handle<Value> Table::RowSpacingGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Table>(info.This())->_actor;

		return scope.Close(
			Integer::New(mx_table_get_row_spacing(MX_TABLE(instance)))
		);
	}

	void Table::RowSpacingSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Table>(info.This())->_actor;

			mx_table_set_row_spacing(MX_TABLE(instance), value->ToInteger()->Value());
		}
	}

	/* Methods */
	Handle<Value> Table::Add(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsObject() && args[1]->IsNumber() && args[2]->IsNumber()) {
			gint row, col;
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
			ClutterActor *actor = ObjectWrap::Unwrap<Actor>(args[0]->ToObject())->_actor;

			/* Get row and column */
			row = args[1]->ToInteger()->Value();
			col = args[2]->ToInteger()->Value();

			mx_table_add_actor(MX_TABLE(instance), CLUTTER_ACTOR(actor), row, col);
		}

		return args.This();
	}

}
