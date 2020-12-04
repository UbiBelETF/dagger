#include "Tilemap.h"

String Filename(String path_) {
	String base_name = path_.substr(path_.find_last_of("\//") + 1);
	size_t const dot = base_name.find_first_of('.');
	String filename = base_name.substr(0, dot);
	return filename;
}
