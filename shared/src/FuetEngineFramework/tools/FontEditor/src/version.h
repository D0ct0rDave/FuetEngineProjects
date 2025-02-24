#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2025";
	static const double UBUNTU_VERSION_STYLE = 25.02;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 6;
	static const long BUILD = 604;
	static const long REVISION = 3361;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 604;
	#define RC_FILEVERSION 0,6,604,3361
	#define RC_FILEVERSION_STRING "0, 6, 604, 3361\0"
	static const char FULLVERSION_STRING[] = "0.6.604.3361";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 4;
	

}
#endif //AutoVersionH
