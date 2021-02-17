#include <X11/Xlib.h>

Display *display;

int main()
{
	  display = XOpenDisplay(NULL);
	    XCloseDisplay(display);
	      return 0;
}
