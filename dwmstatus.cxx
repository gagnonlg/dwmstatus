#include <chrono>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <thread>

#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem::v1;

#include <X11/Xlib.h>

using namespace std::chrono_literals;

class XDisplay {
public:
	XDisplay();
private:
	const std::unique_ptr<Display, int(*)(Display*)> ptr;
};

XDisplay::XDisplay() :
	ptr(std::unique_ptr<Display, int(*)(Display*)>
	    (XOpenDisplay(nullptr), XCloseDisplay))
{
	if (!ptr)
		throw std::runtime_error("Unable to open display");
}

std::ifstream open_ifstream(filesystem::path& path)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	file.open(path);
	return file;
}

int main()
{
	XDisplay dpy;
	for (;;std::this_thread::sleep_for(1s)) {
	}
}
