#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "10";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2012";
	static const double UBUNTU_VERSION_STYLE = 12.09;
	
	//Software Status
	static const char STATUS[] = "";
	static const char STATUS_SHORT[] = "";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 10;
	static const long BUILD = 3215;
	static const long REVISION = 17578;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3215;
	#define RC_FILEVERSION 2,10,3215,17578
	#define RC_FILEVERSION_STRING "2, 10, 3215, 17578\0"
	static const char FULLVERSION_STRING[] = "2.10.3215.17578";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 15;
	

}
#endif //AutoVersionH
