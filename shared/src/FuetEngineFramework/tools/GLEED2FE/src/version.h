#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.12;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 0;
	static const long BUILD = 46;
	static const long REVISION = 246;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 46;
	#define RC_FILEVERSION 0,0,46,246
	#define RC_FILEVERSION_STRING "0, 0, 46, 246\0"
	static const char FULLVERSION_STRING[] = "0.0.46.246";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 46;
	

}
#endif //VERSION_H
