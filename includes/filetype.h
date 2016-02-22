#ifndef __FILETYPE_H_INCLUDED__ 
#define __FILETYPE_H_INCLUDED__ 

#include <magic.h>

class FileType {
public:
	static FileType get_instance();
	const char* get_file_type(char* file_name);
	~FileType();
private:
	FileType();
	static FileType instance_;
	magic_t magic_;
};

#endif