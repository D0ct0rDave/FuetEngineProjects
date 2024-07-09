#ifndef AutoVersionH
#define AutoVersionH

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2024";
	static const double UBUNTU_VERSION_STYLE = 24.07;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 5;
	static const long BUILD = 599;
	static const long REVISION = 3330;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 599;
	#define RC_FILEVERSION 0,5,599,3330
	#define RC_FILEVERSION_STRING "0, 5, 599, 3330\0"
	static const char FULLVERSION_STRING[] = "0.5.599.3330";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 99;
	

}
#endif //AutoVersionH
