#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <mx/mx.h>

#include "../clutter.hpp"
#include "adjustment.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Persistent<FunctionTemplate> Adjustment::constructor;

	Adjustment::Adjustment() : ObjectWrap() {}

	void Adjustment::Initialize(Handle<Object> target)
	{
		/* Initializing Adjustment template  */
		Local<FunctionTemplate> tpl = FunctionTemplate::New();
		Local<String> name = String::NewSymbol("Adjustment");

		/* Constructor */
		constructor = Persistent<FunctionTemplate>::New(tpl);
		constructor->InstanceTemplate()->SetInternalFieldCount(1);
		constructor->SetClassName(name);

		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("value"), Adjustment::ValueGetter, Adjustment::ValueSetter);
		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("lower"), Adjustment::LowerGetter, Adjustment::LowerSetter);
		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("upper"), Adjustment::UpperGetter, Adjustment::UpperSetter);
		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("step_increment"), Adjustment::StepIncrementGetter, Adjustment::StepIncrementSetter);
		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("page_increment"), Adjustment::PageIncrementGetter, Adjustment::PageIncrementSetter);
		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("pagesize"), Adjustment::PageSizeGetter, Adjustment::PageSizeSetter);

		constructor->InstanceTemplate()->SetAccessor(String::NewSymbol("elastic"), Adjustment::ElasticGetter, Adjustment::ElasticSetter);

		target->Set(name, constructor->GetFunction());
	}

	Local<Object> Adjustment::New(MxAdjustment *adjust)
	{
		HandleScope scope;

		Adjustment *obj = new Adjustment();
		Local<Object> ObjectInstance = Adjustment::constructor->GetFunction()->NewInstance();
		obj->Wrap(ObjectInstance);
		obj->_adjust = adjust;

		return ObjectInstance;
	}

	/* ECMAScript constructor */
	Handle<Value> Adjustment::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Adjustment* obj = new Adjustment();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}

	Handle<Value> Adjustment::ValueGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

		return scope.Close(
			Number::New(mx_adjustment_get_value(adjust->_adjust))
		);
	}

	void Adjustment::ValueSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

			mx_adjustment_set_value(adjust->_adjust, value->NumberValue());
		}
	}

	Handle<Value> Adjustment::LowerGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

		return scope.Close(
			Number::New(mx_adjustment_get_lower(adjust->_adjust))
		);
	}

	void Adjustment::LowerSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

			mx_adjustment_set_lower(adjust->_adjust, value->NumberValue());
		}
	}

	Handle<Value> Adjustment::UpperGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

		return scope.Close(
			Number::New(mx_adjustment_get_upper(adjust->_adjust))
		);
	}

	void Adjustment::UpperSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

			mx_adjustment_set_upper(adjust->_adjust, value->NumberValue());
		}
	}

	Handle<Value> Adjustment::StepIncrementGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

		return scope.Close(
			Number::New(mx_adjustment_get_step_increment(adjust->_adjust))
		);
	}

	void Adjustment::StepIncrementSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

			mx_adjustment_set_step_increment(adjust->_adjust, value->NumberValue());
		}
	}

	Handle<Value> Adjustment::PageIncrementGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

		return scope.Close(
			Number::New(mx_adjustment_get_page_increment(adjust->_adjust))
		);
	}

	void Adjustment::PageIncrementSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

			mx_adjustment_set_page_increment(adjust->_adjust, value->NumberValue());
		}
	}

	Handle<Value> Adjustment::PageSizeGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

		return scope.Close(
			Number::New(mx_adjustment_get_page_size(adjust->_adjust))
		);
	}

	void Adjustment::PageSizeSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

			mx_adjustment_set_page_size(adjust->_adjust, value->NumberValue());
		}
	}

	Handle<Value> Adjustment::ElasticGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

		return scope.Close(
			Boolean::New(mx_adjustment_get_elastic(adjust->_adjust))
		);
	}

	void Adjustment::ElasticSetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsBoolean()) {
			Adjustment *adjust = ObjectWrap::Unwrap<Adjustment>(info.This());

			mx_adjustment_set_elastic(adjust->_adjust, value->ToBoolean()->Value());
		}
	}
}
