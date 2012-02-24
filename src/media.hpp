#ifndef NODE_CLUTTER_MEDIA_H_
#define NODE_CLUTTER_MEDIA_H_

#include <clutter/clutter.h>
#include <v8.h>
#include <node.h>

namespace clutter {

class Media : public Actor {
public:
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	Media();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> LoadFile(const v8::Arguments& args);
	static v8::Handle<v8::Value> LoadFileURI(const v8::Arguments& args);
	static v8::Handle<v8::Value> Play(const v8::Arguments& args);
	static v8::Handle<v8::Value> Pause(const v8::Arguments& args);
};

}

#endif
