/*
 * =====================================================================================
 *
 *       Filename:  headers.h
 *
 *    Description:  An attempt to consolidate and order our header files
 *
 *        Version:  1.0
 *        Created:  07/04/2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yamamushi (Jon Rumion)
 *   Organization:  The ASCII Project
 *
 *	  License:  GPLv3
 *
 *	  Copyright 2012 Jonathan Rumion
 *
 *   This file is part of The ASCII Project.
 *
 *   The ASCII Project is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   The ASCII Project is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with The ASCII Project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * =====================================================================================
 */


// Standard headers first
#include <arpa/inet.h>   /* ntohl htonl */
#include <string.h>      /* memcpy */
#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <map>


// Our namespaces
using namespace std;


// Our global constants
#include "constants.h"


// 3rd party local headers
#include "libtcod/libtcod.hpp"
#include "tpl.h"

// 3rd party system level library headers
#include <Sockets/TcpSocket.h>
#include <Sockets/ISocketHandler.h>
#include <Sockets/StdoutLog.h>
#include <Sockets/SocketHandler.h>
#include <Sockets/TcpSocket.h>
#include <Sockets/ListenSocket.h>
#include <Sockets/Utility.h>
#include <Sockets/Parse.h>



// Our most important objects
#include "Tiles.h"
#include "TileMap.h"
#include "Entities.h"
#include "EntityMap.h"


// Our standalone engine libraries
#include "FovLib.h"


// Temporary Dungeon Generator
#include "Dungeon.h"


// This will be rewritten "soon"
#include "Keyboard.h"

// Experimental
#include "NetworkLib.h"
#include "ServerSocket.h"
#include "ClientSocket.h"




// Nothing should depend on our graphics library, ever.
#include "RenderMap.h"

#include "ClientMap.h"

#include "Graphics.h"































