#ifndef NODE_CLUTTER_TEXTURE_H_
#define NODE_CLUTTER_TEXTURE_H_

#include <v8.h>
#include <node.h>

namespace clutter {

class Texture : public Actor {
public:
	static void Initialize(v8::Handle<v8::Object> target);
	static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

protected:
	Texture();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> LoadFile(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetLoadAsync(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetLoadAsync(const v8::Arguments& args);
	static v8::Handle<v8::Value> KeepAspectRatio(const v8::Arguments& args);
};

}

#endif
