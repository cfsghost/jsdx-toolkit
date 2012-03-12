#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <mx/mx.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../clutter.hpp"
#include "../actor.hpp"
#include "widget.hpp"
#include "bin.hpp"
#include "entry.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Entry::Entry() : Widget() {
		_actor = mx_entry_new();
	}

	void Entry::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Entry");

		/* Methods */
		Entry::PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Entry::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Widget::PrototypeMethodsInit(constructor_template);
		Bin::PrototypeMethodsInit(constructor_template);

		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("text"), Entry::TextGetter, Entry::TextSetter);
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("passwordChar"), Entry::PasswordCharGetter, Entry::PasswordCharSetter);
	}

	/* ECMAScript constructor */
	Handle<Value> Entry::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Entry* obj = new Entry();
		obj->Wrap(args.This());

		if (args[0]->IsString()) {
			mx_entry_set_text(MX_ENTRY(obj->_actor), *String::Utf8Value(args[0]->ToString()));
		}

		return scope.Close(args.This());
	}

	/* Accessor */
	Handle<Value> Entry::TextGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Entry>(info.This())->_actor;

		return scope.Close(
			String::New(mx_entry_get_text(MX_ENTRY(instance)))
		);
	}

	void Entry::TextSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Entry>(info.This())->_actor;

			mx_entry_set_text(MX_ENTRY(instance), *String::Utf8Value(value->ToString()));
		}
	}

	Handle<Value> Entry::PasswordCharGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;
		ClutterActor *instance = ObjectWrap::Unwrap<Entry>(info.This())->_actor;
		char c[2] = { 0 };

		c[0] = mx_entry_get_password_char(MX_ENTRY(instance));

		return scope.Close(
			String::New(c)
		);
	}

	void Entry::PasswordCharSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsString()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Entry>(info.This())->_actor;
			char *s = strdup(*String::Utf8Value(value->ToString()));

			mx_entry_set_password_char(MX_ENTRY(instance), *s);

			delete s;
		}
	}

}
