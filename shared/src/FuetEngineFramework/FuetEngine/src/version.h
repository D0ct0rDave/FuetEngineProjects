#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2014";
	static const double UBUNTU_VERSION_STYLE = 14.09;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 4;
	static const long MINOR = 3;
	static const long BUILD = 5780;
	static const long REVISION = 26600;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4958;
	#define RC_FILEVERSION 4,3,5780,26600
	#define RC_FILEVERSION_STRING "4, 3, 5780, 26600\0"
	static const char FULLVERSION_STRING[] = "4.3.5780.26600";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 80;
	

}
#endif //AutoVersionH
