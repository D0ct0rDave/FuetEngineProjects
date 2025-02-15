#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.12;
	
	//Software Status
	static const char STATUS[] = "";
	static const char STATUS_SHORT[] = "";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 1787;
	static const long REVISION = 9800;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1787;
	#define RC_FILEVERSION 1,6,1787,9800
	#define RC_FILEVERSION_STRING "1, 6, 1787, 9800\0"
	static const char FULLVERSION_STRING[] = "1.6.1787.9800";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 87;
	

}
#endif //VERSION_H
