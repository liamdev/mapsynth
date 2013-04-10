============================
Map Synthesiser
============================

3rd year project system for generating city networks in an example-based manner.

The algorithm is based on the idea used in patch-based texture synthesis. The input maps are broken up into fragments, then pieced together in larger combinations in such a way that a cost function at their borders is minimised. While standard texture synthesis uses pixel data, the map synthesiser uses road vector data.

Program compilation requires the Boost Filesystem library found at:
http://www.boost.org/doc/libs/1_53_0/libs/filesystem/doc/index.htm
Code was written and compiled against Boost 1.53. No guarantees are provided for other versions of the library.

============================
Program Instructions
============================

Input:
-----------------
All SVG maps within the "res" directory are automatically loaded into the program as input. The maps must be directly in the directory; the automatic input is not recursive, and so will not find SVG maps stored within further directories.

Example maps are provided in the "maps" folder within the resource directory, for testing purposes.

Arguments can be provided to modify the behaviour of the synthesiser. Usage:

./mapsynth [window] [offset] [output]

Window size specifies the dimensions of the individual fragments extracted from the input maps (i.e. the width and height of the 'puzzle pieces').
Offset specifies the distance between extracted fragments (e.g. an offset of 1 will extract a new fragment at every pixel). It is recommended that this is not set to extremely low values, as halving the offset size quadruples the number of fragments which the program must handle.
Output controls the number of 'pieces' the program puts together to form the resulting maps.

Output:
-----------------

SVG maps are output into the root directory of the repository (likely to change).

An evaluation file is also written to the root directory, containing statistics for each of the generated maps, as well as the input maps.