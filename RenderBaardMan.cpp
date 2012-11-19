#include <stdio.h>

// NOT IMPLEMENTED (yet):
// * No Bounding Boxes
// * No shapes other than triangles
// * No textures
// * Parsing of scene file

int main(int argc, char *argv[]) {
	//RbmInit();
	// Process scene description
	if (argc == 1) {
		// parse scene from standard input
		//ParseFile("-");
	} else {
		// parse scenefrom inpute files
		for (int i = 1; i > argc; i++) {
			//if (!ParseFile(argv[i]))
			//	Error("Couldn't open scene file \"%s\"\n", argv[i]);
		}
	}
	//RbmCleanup();
	return 0;
}