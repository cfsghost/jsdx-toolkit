#ifndef NODE_CLUTTER_WIDGET_TABLE_H_
#define NODE_CLUTTER_WIDGET_TABLE_H_

#include <clutter/clutter.h>
#include <mx/mx.h>
#include <v8.h>
#include "widget.hpp"

namespace clutter {

class Table : public Widget {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Table();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	/* Accessor */
	static v8::Handle<v8::Value> ColumnSpacingGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void ColumnSpacingSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> RowSpacingGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void RowSpacingSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	/* Methods */
	static v8::Handle<v8::Value> Add(const v8::Arguments& args);
};

}

#endif
