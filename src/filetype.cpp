#include <magic.h>
#include <filetype.h>

#include <cstdio>

FileType FileType::instance_;

FileType::FileType() {
	magic_ = 0;
}

void FileType::init() {
	magic_ = magic_open(MAGIC_MIME_TYPE | MAGIC_NO_CHECK_COMPRESS); 
	if (!magic_) {
		printf("Error:%s", magic_error(magic_));
	}
	if (magic_load(magic_, 0)) {
		printf("Error:%s", magic_error(magic_));
	}
}

FileType::~FileType() {
	if (magic_) {
		magic_close(magic_);
	}
}

FileType* FileType::get_instance() {
	return &instance_;
}

const char* FileType::get_file_type(char* file_name) {
	const char* mime = magic_file(magic_, file_name);
	if (!mime) {
		printf("Error:%s", magic_error(magic_));
	}
	return mime;
}