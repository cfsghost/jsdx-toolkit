#include <stdio.h>
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

	void X11::setWindowType(Display *disp, Window w, X11WindowType wtype)
	{
		Atom wt_atom;
		Atom type_atom;

		switch(wtype) {
		case X11_WINDOW_TYPE_NORMAL:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_NORMAL", False);
			break;
		case X11_WINDOW_TYPE_DESKTOP:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
			break;
		case X11_WINDOW_TYPE_DOCK:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DOCK", False);
			break;
		case X11_WINDOW_TYPE_TOOLBAR:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
			break;
		case X11_WINDOW_TYPE_MENU:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_MENU", False);
			break;
		case X11_WINDOW_TYPE_UTILITY:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_UTILITY", False);
			break;
		case X11_WINDOW_TYPE_SPLASH:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_SPLASH", False);
			break;
		case X11_WINDOW_TYPE_DIALOG:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DIALOG", False);
			break;
		default:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_NORMAL", False);
		}

		wt_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", False);

		XChangeProperty(disp, w,
			wt_atom,
			XA_ATOM, 32, PropModeReplace,
			(unsigned char *)&type_atom, 1);
	}
}
