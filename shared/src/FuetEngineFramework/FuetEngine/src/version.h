#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.12;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 0;
	static const long BUILD = 2135;
	static const long REVISION = 6366;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1187;
	#define RC_FILEVERSION 1,0,2135,6366
	#define RC_FILEVERSION_STRING "1, 0, 2135, 6366\0"
	static const char FULLVERSION_STRING[] = "1.0.2135.6366";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 35;
	

}
#endif //VERSION_H
