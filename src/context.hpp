#ifndef JSDX_TOOLKIT_CONTEXT_H_
#define JSDX_TOOLKIT_CONTEXT_H_

#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

	struct gcontext {
		int max_priority;
		int nfds;
		int allocated_nfds;
		GPollFD *fds;
		GMainContext *gc;

		uv_poll_t *poll_handles;
	};

	struct gcontext_pollfd {
		GPollFD *pfd;
	};

	class Context {
	public:
		Context();
		void Init();
		void Uninit();

	protected:
		static void poll_cb(uv_poll_t *handle, int status, int events);
		static void prepare_cb(uv_prepare_t *handle, int status);
		static void check_cb(uv_check_t *handle, int status);
	};

}

#endif
