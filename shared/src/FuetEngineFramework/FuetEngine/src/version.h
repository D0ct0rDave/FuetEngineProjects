#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2025";
	static const double UBUNTU_VERSION_STYLE = 25.02;
	
	//Software Status
	static const char STATUS[] = "";
	static const char STATUS_SHORT[] = "";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 1275;
	static const long REVISION = 6991;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1275;
	#define RC_FILEVERSION 1,1,1275,6991
	#define RC_FILEVERSION_STRING "1, 1, 1275, 6991\0"
	static const char FULLVERSION_STRING[] = "1.1.1275.6991";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 75;
	

}
#endif //AutoVersionH
