#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.04;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 0;
	static const long BUILD = 1063;
	static const long REVISION = 391;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 88;
	#define RC_FILEVERSION 0,0,1063,391
	#define RC_FILEVERSION_STRING "0, 0, 1063, 391\0"
	static const char FULLVERSION_STRING[] = "0.0.1063.391";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 63;
	

}
#endif //VERSION_H
