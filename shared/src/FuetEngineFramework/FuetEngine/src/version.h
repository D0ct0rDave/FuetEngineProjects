#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2011";
	static const double UBUNTU_VERSION_STYLE = 11.08;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 7;
	static const long BUILD = 2831;
	static const long REVISION = 10229;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1883;
	#define RC_FILEVERSION 1,7,2831,10229
	#define RC_FILEVERSION_STRING "1, 7, 2831, 10229\0"
	static const char FULLVERSION_STRING[] = "1.7.2831.10229";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 31;
	

}
#endif //AutoVersionH
