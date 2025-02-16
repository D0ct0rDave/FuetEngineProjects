#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.12;
	
	//Software Status
	static const char STATUS[] = "";
	static const char STATUS_SHORT[] = "";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 9;
	static const long BUILD = 3178;
	static const long REVISION = 17396;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3178;
	#define RC_FILEVERSION 2,9,3178,17396
	#define RC_FILEVERSION_STRING "2, 9, 3178, 17396\0"
	static const char FULLVERSION_STRING[] = "2.9.3178.17396";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 78;
	

}
#endif //VERSION_H
