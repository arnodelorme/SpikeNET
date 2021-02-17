// Copyright (C) 1997-2002 Arnaud Delorme, arno@salk.edu
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

// $Log: archive.cpp,v $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allDefs.h"
#include "FileFolder.h"


int nbFiles = 41;
char *fileList[] = {
":Data:Convolutions:FS4-OR0.off",
":Data:Convolutions:FS4-OR0.on",
":Data:Convolutions:FS4-OR135.off",
":Data:Convolutions:FS4-OR135.on",
":Data:Convolutions:FS4-OR180.off",
":Data:Convolutions:FS4-OR180.on",
":Data:Convolutions:FS4-OR225.off",
":Data:Convolutions:FS4-OR225.on",
":Data:Convolutions:FS4-OR270.off",
":Data:Convolutions:FS4-OR270.on",
":Data:Convolutions:FS4-OR315.off",
":Data:Convolutions:FS4-OR315.on",
":Data:Convolutions:FS4-OR45.off",
":Data:Convolutions:FS4-OR45.on",
":Data:Convolutions:FS4-OR90.off",
":Data:Convolutions:FS4-OR90.on",
":Data:Convolutions:Inhib",
":Data:Convolutions:OffCenter",
":Data:Convolutions:OffCenter5x5",
":Data:Convolutions:OffCenter5x5_2",
":Data:Convolutions:OnCenter",
":Data:Convolutions:zhaoping_OR0",
":Data:Convolutions:zhaoping_OR0i",
":Data:Convolutions:zhaoping_OR135",
":Data:Convolutions:zhaoping_OR135i",
":Data:Convolutions:zhaoping_OR45",
":Data:Convolutions:zhaoping_OR45i",
":Data:Convolutions:zhaoping_OR90",
":Data:Convolutions:zhaoping_OR90i",
":Data:net1:net_env",
":Data:net1:net_names",
":Data:net1:net_projs",
":Data:net1Z:net_env",
":Data:net1Z:net_names",
":Data:net1Z:net_projs",
":Data:net8:net_env",
":Data:net8:net_names",
":Data:net8:net_projs",
":Data:net8Z:net_env",
":Data:net8Z:net_names",
":Data:net8Z:net_projs" };

#define EXTRACT

int main(int argc, char *argv[]) {
#ifdef EXTRACT
	FileFolder *testing = new FileFolder(ARCHIVE_NAME);
	testing->extractAll();
#else
	FileFolder *testing = new FileFolder();

	for (int i=0; i<nbFiles; i++) {
		if (testing->addFile( fileList[ i ]) != -1) printf("archiving file %s\n", fileList[ i ]);
		else							     printf("unable to find file %s\n", fileList[ i ]);
	}
	testing->flush( ARCHIVE_NAME);
#endif
}
		
		
		
		
		