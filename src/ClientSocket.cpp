/*
 * =====================================================================================
 *
 *       Filename:  ClientSocket.cpp
 *
 *    Description:  Wrapper for Boost::Asio library as used in the Client.
 *
 *        Version:  1.0
 *        Created:  07/20/2012 10:28 PM
 *       Revision:  Rev 1 - 07/26/2012
 *       Compiler:  clang++
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


using boost::asio::ip::tcp;



ClientSession::ClientSession(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator, ClientMap *client, GraphicsTCOD *screen) : io_service_(io_service), socket_(io_service)
{
    clientMap = client;
    output = screen;
    packer = new ClientMapPacker();
    
    tmp[0] = 0;
    
    m_pause = false;
    sent = false;
    
    cout << "Connected\n";
    
    boost::asio::async_connect(socket_, endpoint_iterator, boost::bind(&ClientSession::read_map, this, boost::asio::placeholders::error));
}

void ClientSession::read_map(const boost::system::error_code& error)
{
    block_while_paused();
    
    if(!error)
    {

        buf = new boost::array<char, MAP_PACKET_SIZE>;
                
        if(sent == true)
        {
            sent = false;
            
            clientMap->cleanMap();
            clientMap->clearIgnore();
            output->render();
            sprintf(tmp,"%d", '0');
            
            boost::asio::async_read(socket_, boost::asio::buffer(buf, MAP_PACKET_SIZE), boost::bind(&ClientSession::ignoreMap, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
        else
        {
        sprintf(tmp,"%d", '0');
        
        boost::asio::async_read(socket_, boost::asio::buffer(buf, MAP_PACKET_SIZE), boost::bind(&ClientSession::callNewMap, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
    }
    else
    {
        close();
    }
}


void ClientSession::callNewMap(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    block_while_paused();
    if(!error)
    {
        //printf("bytes transferred: %d\n", (int)bytes_transferred);
        
        boost::asio::const_buffer conBuf = boost::asio::buffer(*buf);
        const unsigned char* tmpUnpack = boost::asio::buffer_cast<const unsigned char*>(conBuf);
        
        for(int x = 0; x < (bytes_transferred/TILE_PACKET_SIZE); x++)
        {
            char *unpack = new char[TILE_PACKET_SIZE];
                        
            memset(unpack, '0', TILE_PACKET_SIZE);
            memcpy(unpack, &tmpUnpack[x*TILE_PACKET_SIZE], TILE_PACKET_SIZE);
            
            if((unpack[0] != '0') && (unpack[TILE_PACKET_SIZE/2] != '0')  && (unpack[TILE_PACKET_SIZE] != '0'))
                packer->unpackFromNet(clientMap, (unsigned char*)unpack, output);
            
            free(unpack);
        }
        
        
        output->render();
        
        free(buf);
        
        boost::asio::async_write(socket_, boost::asio::buffer(&tmp, 2), boost::bind(&ClientSession::read_map, this, boost::asio::placeholders::error));
    }
    else
    {
        close();
    }    
    
}



void ClientSession::ignoreMap(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        
        boost::asio::const_buffer conBuf = boost::asio::buffer(*buf);
        const unsigned char* tmpUnpack = boost::asio::buffer_cast<const unsigned char*>(conBuf);
        
        for(int x = 0; x < (bytes_transferred/TILE_PACKET_SIZE); x++)
        {
            char *unpack = new char[TILE_PACKET_SIZE];
            
            memset(unpack, '0', TILE_PACKET_SIZE);
            memcpy(unpack, &tmpUnpack[x*TILE_PACKET_SIZE], TILE_PACKET_SIZE);
            
            if((unpack[0] != '0') && (unpack[TILE_PACKET_SIZE/2] != '0')  && (unpack[TILE_PACKET_SIZE] != '0'))
            {
                packer->unpackFromNet(clientMap, (unsigned char*)unpack, output);
            }
            
            free(unpack);
        }
        
        clientMap->clearIgnore();
        output->render();
        
        
        free(buf);
        
        boost::asio::async_write(socket_, boost::asio::buffer(&tmp, 2), boost::bind(&ClientSession::read_map, this, boost::asio::placeholders::error));
    }
    else
    {
        close();
    }
}





void ClientSession::sendAPICall(int api)
{
    sprintf(tmp, "%d", api);
    sent = true;
}


void ClientSession::kick(int x)
{
    
}




void ClientSession::close()
{
    io_service_.post(boost::bind(&ClientSession::do_close, this));
}

void ClientSession::do_close()
{
    socket_.close();
}


void ClientSession::block_while_paused()
{
    boost::unique_lock<boost::mutex> lock(m_pause_mutex);
    while(m_pause)
    {
        m_pause_changed.wait(lock);
    }
}


void ClientSession::set_paused(bool new_value)
{
    {
        boost::unique_lock<boost::mutex> lock(m_pause_mutex);
        m_pause = new_value;
    }
    
    m_pause_changed.notify_all();
    
}



















