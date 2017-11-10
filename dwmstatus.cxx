#include <chrono>
#include <memory>
#include <stdexcept>
#include <thread>

#include <X11/Xlib.h>

using namespace std::chrono_literals;

class XDisplay : std::unique_ptr<Display, int(*)(Display*)> {
public:
	XDisplay();
};

XDisplay::XDisplay() :
	std::unique_ptr<Display, int(*)(Display*)>
	(XOpenDisplay(nullptr), XCloseDisplay)
{
	if (get() == nullptr)
		throw std::runtime_error("Unable to open display");
}

int main()
{
	XDisplay dpy;
	for (;;std::this_thread::sleep_for(1s)) {
	}
}
