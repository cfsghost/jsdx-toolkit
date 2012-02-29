#ifndef NODE_CLUTTER_STAGE_H_
#define NODE_CLUTTER_STAGE_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class Stage : public Container {
public:
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	Stage();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetUseAlpha(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetColor(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetCursor(const v8::Arguments& args);
	static v8::Handle<v8::Value> Fullscreen(const v8::Arguments& args);

	static v8::Handle<v8::Value> TitleGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	static void TitleSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

}

#endif
