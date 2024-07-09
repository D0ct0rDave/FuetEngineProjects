#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2024";
	static const double UBUNTU_VERSION_STYLE = 24.07;
	
	//Software Status
	static const char STATUS[] = "";
	static const char STATUS_SHORT[] = "";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 1212;
	static const long REVISION = 6649;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1212;
	#define RC_FILEVERSION 1,1,1212,6649
	#define RC_FILEVERSION_STRING "1, 1, 1212, 6649\0"
	static const char FULLVERSION_STRING[] = "1.1.1212.6649";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 12;
	

}
#endif //AutoVersionH
