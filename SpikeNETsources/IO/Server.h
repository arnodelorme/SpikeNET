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

// $Log: Server.h,v $

#include "Map.h"
#include "MapBuffer.h"

#ifndef _SERVER
#define _SERVER

class superSpikeClass2;

// receiving messages
// ------------------
class Server
{
    char     *hostName;

public:
	char *getHostName() { return hostName; }
	
#if SYSTEME == UNIX
	void treatExternal() { }
	void launch(int port, superSpikeClass2 *parent0) { }
    void blockSignals() { }
    void unblockSignals() { }
    Server() { hostName = NULL; }
	~Server() { }
#else
	void treatExternal() { }
	void launch(int port, superSpikeClass2 *parent0) { }
    void blockSignals() { }
    void unblockSignals() { }
    Server() { hostName = NULL; }
	~Server() { }
#endif
};

#endif
