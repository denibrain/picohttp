#include <magic.h>
#include <filetype.h>

FileType FileType::instance_;

FileType::FileType() {
	magic_ = magic_open(MAGIC_MIME_TYPE); 
	magic_load(magic_, 0);
	magic_compile(magic_, 0);
}

FileType::~FileType() {
	magic_close(magic_);
}

FileType FileType::get_instance() {
	return instance_;
}

const char* FileType::get_file_type(char* file_name) {
	return magic_file(magic_, file_name);
}