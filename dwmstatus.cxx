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

std::ifstream open_ifstream(filesystem::path&& path)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	file.open(path);
	return file;
}

filesystem::path get_battery_info_path()
{
	filesystem::path dirpath;

	for (int i = 0; i < 256 || dirpath.empty(); i++) {
		std::ostringstream path_stream;
		path_stream << "/sys/class/power_supply/BAT" << i;
		if (filesystem::is_directory(path_stream.str()))
			dirpath.assign(path_stream.str());
	}

	if (dirpath.empty())
		throw std::runtime_error("Unable to find battery info");

	return dirpath;
}

class BatteryInfo {
public:
	BatteryInfo(filesystem::path&& base_path);
	double energy_now();
	double energy_full();
private:
	std::ifstream _file_energy_now;
	std::ifstream _file_energy_full;
};

BatteryInfo::BatteryInfo(filesystem::path&& base_path) :
	_file_energy_now(open_ifstream(base_path / "energy_now")),
	_file_energy_full(open_ifstream(base_path / "energy_full"))
{
}

double BatteryInfo::energy_now()
{
	double energy_now;
	_file_energy_now.seekg(0);
	_file_energy_now >> energy_now;
	return energy_now;
}

double BatteryInfo::energy_full()
{
	double energy_full;
	_file_energy_full.seekg(0);
	_file_energy_full >> energy_full;
	return energy_full;
}

int main()
{
	XDisplay dpy;
	for (;;std::this_thread::sleep_for(1s)) {
	}
}
