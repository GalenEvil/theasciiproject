/*
 * =====================================================================================
 *
 *       Filename:  Entities.cpp
 *
 *    Description:  This object defines the behavior of our "Entities", ie
 *                  The necessary functions for an entity to exist on the "Map"
 *
 *        Version:  1.0
 *        Created:  07/03/2012 03:41:29 AM
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

#include "Headers.h"

// Temporary location for these variables, they will be moved elsewhere
// When DB library code is integrated (soon-ish).

unsigned int UIDList = 0;

void Entity::init_entity(wchar_t *p)
{
    
    
    symbol = p;
    X = 0;
    Y = 0;
    
    H = 0;
    S = 0;
    V = 0;
    
    R = 0;
    G = 0;
    B = 0;
    
    TypeID = 0;
    SubTypeID = 0;
    UID = UIDList + 1;
    UIDList++;
    
    
    initialized = false;
    clientActive = false;
    
    int x, y;
    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
            fov[x][y] = false;
        }
    }
}


void Entity::setEntName(std::string entName_)
{
    
    entName = entName_;
    
}


std::string Entity::getEntName()
{
    
    return entName;
    
}


bool Entity::move(int dx, int dy)
{
    
    if (initialized) {
        if ((world->virtMap[(dx + X)][(dy + Y)]->blocked)){
            entMap->refreshEntityMap();
            
            if (clientActive) {
                
                clientFovSync();
            }
            
            return false;
        }
        else {
            X += dx;
            Y += dy;
            
            entMap->refreshEntityMap();
            
            if (clientActive) {
                
                clientFovSync();
            }
            
            
            return true;
        }
    } else {
        return false;
    }
    
}


bool Entity::digTile(int dx, int dy)
{
    
    if (initialized) {
        if ((world->virtMap[(dx + X)][(dy + Y)]->blocked)){
            entMap->contextMap->removeTile(dx + X, dy + Y);
            //entMap->refreshTileMap();
            entMap->refreshEntityMap();
            
            if (clientActive) {
                
                clientFovSync();
            }
            
            return true;
        }
        else {
            
            entMap->refreshEntityMap();
            
            if (clientActive) {
                
                clientFovSync();
            }
            
            
            return false;
        }
    } else {
        return false;
    }
}





bool Entity::placeTile(int dx, int dy)
{
    
    if (initialized) {
        if ((world->virtMap[(dx + X)][(dy + Y)]->blocked)){
            entMap->refreshEntityMap();
            
            if (clientActive) {
                
                clientFovSync();
            }
            
            return false;
        }
        else {
            
            entMap->contextMap->placeTile(dx + X, dy + Y);
            //entMap->refreshTileMap();
            entMap->refreshEntityMap();
            
            if (clientActive) {
                
                clientFovSync();
            }
            
            
            return true;
        }
    } else {
        return false;
    }
}



void Entity::clientFovSync(){
    
    
    refreshFov();
    
    
    int pX, pY;
    pX = X;
    pY = Y;
    
    int cX, cY;
    cX = MAP_WIDTH/2;
    cY= MAP_HEIGHT/2;
    
    int offset = 20;
    
    
    
    
    
    
    //   if(fov[x][y] == true)
    
    int x = pX-offset;
    for(int iX = cX-offset; iX < cX+offset; iX++ )
    {
        int y = pY-offset;
        for(int iY = cY-offset; iY < cY+offset; iY++ )
        {
            
            if(y > 0 && y < MAP_HEIGHT && x > 0 && x < MAP_WIDTH)
                
            {
                
                cMap->cMap[iX][iY]->symbol = rMap->getSymbol(x, y);
                cMap->cMap[iX][iY]->H = rMap->returnH(x, y);
                cMap->cMap[iX][iY]->HD = rMap->returnHD(x, y);
                cMap->cMap[iX][iY]->S = rMap->returnS(x, y);
                cMap->cMap[iX][iY]->SD = rMap->returnSD(x, y);
                cMap->cMap[iX][iY]->V = rMap->returnV(x, y);
                cMap->cMap[iX][iY]->VD = rMap->returnVD(x, y);
                cMap->cMap[x][y]->occupied = rMap->returnOccupied(x, y);
                
                
                
                if(fov[x][y] == true)
                {
                    
                    cMap->cMap[iX][iY]->visible = true;
                    cMap->cMap[iX][iY]->explored = true;
                    
                }
                else
                {
                    cMap->cMap[iX][iY]->visible = false;
                    cMap->cMap[iX][iY]->occupied = false;
                }
                
                
            }
            
            y++;
        }
        
        
        x++;
    }
    //    }
    
    
    /*
     for (int x = 0; x < MAP_WIDTH; x++) {
     for (int y = 0; y < MAP_HEIGHT; y++) {
     if (fov[x][y] == true) {
     
     cMap->cMap[x][y]->symbol = rMap->getSymbol(x, y);
     cMap->cMap[x][y]->H = rMap->returnH(x, y);
     cMap->cMap[x][y]->HD = rMap->returnHD(x, y);
     cMap->cMap[x][y]->S = rMap->returnS(x, y);
     cMap->cMap[x][y]->SD = rMap->returnSD(x, y);
     cMap->cMap[x][y]->V = rMap->returnV(x, y);
     cMap->cMap[x][y]->VD = rMap->returnVD(x, y);
     //cMap->cMap[x][y]->explored = true;
     //cMap->cMap[x][y]->occupied = rMap->returnOccupied(x, y);
     //cMap->cMap[x][y]->visible = rMap->returnVisible(x, y);
     
     }
     }
     }
     */
    
    
}

void Entity::refreshFov()
{
    
    FOV->refreshFov(this);
    //clientFovSync();
}

int Entity::posX()
{
    return X;
}

int Entity::posY()
{
    return Y;
}

void Entity::init_in_world(FovLib *fovLib)
{
    
    FOV = fovLib;
    initialized = true;
    world = fovLib->getTileMap();
    
    
    
}

wchar_t *Entity::getSymbol()
{
    
    return symbol;
    
}

void Entity::setSymbol(wchar_t *ch)
{
    symbol = ch;
}

void Entity::setTypeID(int i)
{
    TypeID = i;
}

void Entity::setSubTypeID(int i)
{
    SubTypeID = i;
}

bool Entity::isInitialized()
{
    
    return initialized;
    
}

void Entity::associateClient(RenderMap *RMap)
{
    
    cMap = new ClientMap();
    rMap = RMap;
    clientActive = true;
    
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            fov[x][y] = false;
        }
    }
    
    entMap->refreshEntityMap();
    
    //  clientFovSync();
    //   refreshFov();
}

bool Entity::getAssociated()
{
    
    return clientActive;
    
}

ClientMap *Entity::returnCMap()
{
    
    return cMap;
    
}

void Entity::move_self(int dx, int dy)
{
    
    move(dx, dy);
    
    
};

void Entity::setEntityMap(EntityMap *map)
{
    
    entMap = map;
    entMap->refreshEntityMap();
    
};

/*
 *
 *
 *		Entities
 *
 *	Now we start listing some entity types
 *
 *	Though how this works will likely change in the
 *	future.
 *
 *
 *
 */



Monster::Monster() : Entity((wchar_t *)L"\uFFF7")
{
    
    //Entity((wchar_t *)L"\u263A");
    
}

Player::Player() : Entity((wchar_t *)L"\u263A")
{
    
    // Entity((wchar_t *)L"\u263A");
    H = 0.0;
    S = 0.0;
    V = 1.0;
    
}

Goblin::Goblin() : Entity((wchar_t *)L"\uFFF7")
{
    
    //  Entity((wchar_t *)L"\uFFF7");
    H = 30.0;
    S = 0.58;
    V = 0.40;
    
}
