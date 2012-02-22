#ifndef NODE_CLUTTER_TEXT_H_
#define NODE_CLUTTER_TEXT_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class Text : public Actor {
public:
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	Text();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetColor(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetColor(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetBorderColor(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetBorderColor(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetBorderWidth(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetBorderWidth(const v8::Arguments& args);
};

}

#endif
