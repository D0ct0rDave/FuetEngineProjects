#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "29";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.09;
	
	//Software Status
	static const char STATUS[] = "";
	static const char STATUS_SHORT[] = "";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 1750;
	static const long REVISION = 9592;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1750;
	#define RC_FILEVERSION 1,6,1750,9592
	#define RC_FILEVERSION_STRING "1, 6, 1750, 9592\0"
	static const char FULLVERSION_STRING[] = "1.6.1750.9592";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 50;
	

}
#endif //VERSION_H
