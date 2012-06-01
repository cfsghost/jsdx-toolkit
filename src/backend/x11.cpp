#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "x11.hpp"

namespace JSDXToolkit {

	void X11::setWindowDecorator(Display *disp, Window w, bool enabled)
	{
		unsigned char *data;
		MotifWmHints new_hints;
		MotifWmHints *hints;
		int format;
		unsigned long nitems;
		unsigned long bytes_after;
		Atom type;
		Atom hints_atom = XInternAtom(disp, "_MOTIF_WM_HINTS", False);
		XSetWindowAttributes attr;

		/* Get Current hints and set decorator flags */
		XGetWindowProperty(disp, w,
			hints_atom, 0, sizeof(MotifWmHints) / sizeof(long),
			False, AnyPropertyType, &type, &format, &nitems,
			&bytes_after, &data);

		if (type == None) {
			new_hints.flags = MWM_HINTS_DECORATIONS;
			new_hints.decorations = enabled ? MWM_DECOR_ALL : 0L;
			hints = &new_hints;
		} else {
			hints = (MotifWmHints *)data;
			hints->flags |= MWM_HINTS_DECORATIONS;

			if (enabled) {
				hints->decorations |= MWM_DECOR_ALL;
			} else {
				hints->decorations = 0L;
			}
		}

		XGrabServer(disp);
		XChangeProperty(disp, w,
			hints_atom, hints_atom, 32, PropModeReplace,
			(unsigned char *)hints, sizeof(MotifWmHints) / sizeof(long));
		XUngrabServer(disp);

		if (hints != &new_hints)
			XFree(hints);
	}
}
