#ifndef NODE_CLUTTER_WIDGET_ADJUSTMENT_H_
#define NODE_CLUTTER_WIDGET_ADJUSTMENT_H_

#include <v8.h>
#include <node.h>
#include <mx/mx.h>

namespace clutter {

class Adjustment : public node::ObjectWrap {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Initialize(v8::Handle<v8::Object> target);
	static v8::Local<v8::Object> New(MxAdjustment *adjust);

	MxAdjustment *_adjust;

protected:

	Adjustment();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> ValueGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void ValueSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> LowerGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void LowerSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> UpperGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void UpperSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> StepIncrementGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void StepIncrementSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> PageIncrementGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void PageIncrementSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> PageSizeGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void PageSizeSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	static v8::Handle<v8::Value> ElasticGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void ElasticSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

}

#endif
