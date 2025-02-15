#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2011";
	static const double UBUNTU_VERSION_STYLE = 11.01;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 1;
	static const long BUILD = 120;
	static const long REVISION = 666;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 120;
	#define RC_FILEVERSION 0,1,120,666
	#define RC_FILEVERSION_STRING "0, 1, 120, 666\0"
	static const char FULLVERSION_STRING[] = "0.1.120.666";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 20;
	

}
#endif //AutoVersionH
