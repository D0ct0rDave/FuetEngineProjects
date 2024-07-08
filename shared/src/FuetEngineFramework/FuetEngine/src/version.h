#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2024";
	static const double UBUNTU_VERSION_STYLE = 24.07;
	
	//Software Status
	static const char STATUS[] = "";
	static const char STATUS_SHORT[] = "";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 1208;
	static const long REVISION = 6623;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1208;
	#define RC_FILEVERSION 1,1,1208,6623
	#define RC_FILEVERSION_STRING "1, 1, 1208, 6623\0"
	static const char FULLVERSION_STRING[] = "1.1.1208.6623";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 8;
	

}
#endif //AutoVersionH
