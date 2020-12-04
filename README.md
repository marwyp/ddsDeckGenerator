# ddsDeckGenerator

### This is library for dds Double Dummy Solver
* [DDS - GitHub](https://github.com/dds-bridge)
* Make sure you don't violate its license [DDS - License](https://github.com/dds-bridge/dds/blob/develop/LICENSE)



### To compile code on Linux:
* Make sure ddl.h and libdds.so are in directory
* Use make or make dds_40/dds_for
* Problems with compiling:
	* boost problems:
		* Make sure you've installed boost/thread.hpp
		* Use *sudo apt install libboost-thread-dev* if not
		* Read more [stackoverflow](https://stackoverflow.com/questions/59561902/boost-thread-hpp-no-such-file-or-directory)
	* cannot open shared object file: No such file or directory:
		* *sudo apt install libdds-dev* helps
		* more [link](https://itsfoss.com/solve-open-shared-object-file-quick-tip/), look at **Alternate method to fix ‘cannot open shared object file’ error** paragraph
