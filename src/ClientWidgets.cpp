#include "Headers.h"


using boost::asio::ip::tcp;



// x and y are our positions on the screen, w and h are our width and height.
ScrollBox::ScrollBox( int x, int y, int w, int h, int MaxBuffer, TCODConsole *Console, ClientMap *cMap, GraphicsTCOD *graphicsEngine) : Widget(x,y,w,h)
{
    
    maxBuffer = MaxBuffer;
    textBuffer = new std::vector<std::string>;
    console = Console;
    scrollBuffer = 0;
    acceptCommands = false;
    connected = false;
    
    graphics = graphicsEngine;
    clientMap = cMap;
}



ScrollBox::~ScrollBox()
{
    
    if(!textBuffer->empty())
    {
        textBuffer->clear();
        delete textBuffer;
        
    }
    
    
}


void ScrollBox::setRealPosition(int x, int y)
{
    
    realX = x;
    realY = y;
    
}


void ScrollBox::update(const TCOD_mouse_t k)
{
    
    if (mouse.cx == realX + w - 2 && mouse.cy == realY + 1)
    {
        onUpArrow = true;
        //cout << "Up" << endl;
    }
    else if (mouse.cx == realX + w - 2 && mouse.cy == realY + h - 2 )
    {
        onDownArrow = true;
        //cout << "Down" << endl;
    }
    else
    {
        onUpArrow = false;
        onDownArrow = false;
    }
    
    
    if(k.lbutton)
    {
        onButtonPress();
    }
    
    if(k.wheel_up)
        onScrollUp();
    
    if(k.wheel_down)
        onScrollDown();
}


void ScrollBox::attachConsole(TCODConsole *Console)
{
    console = Console;
}

void ScrollBox::takeCommands(bool accept)
{
    acceptCommands = accept;
}

void ScrollBox::insertText(std::string newText)
{
    
    if(textBuffer)
    {
        if(textBuffer->size() >= maxBuffer)
            textBuffer->erase(textBuffer->begin());
        
        textBuffer->push_back(newText);
    }
    
}


void ScrollBox::render()
{
    
    console->setDefaultBackground(back);
    console->setDefaultForeground(fore);
    console->printFrame(x,y,w,h,true,TCOD_BKGND_SET);
    console->print(x+w-2, y+1, L"\u21E7");
    console->print(x+w-2, y+h-2, L"\u21E9");
    
    console->setColorControl(TCOD_COLCTRL_2, TCODColor(0,255,0), TCODColor(0,0,0));
    
    
    if(!textBuffer->empty())
    {
        
        std::string checkForCommand = textBuffer->back();
        
        if(acceptCommands)
        {
            if(checkForCommand == "/exit")
            {
                exit(0);
            }
            
            if(checkForCommand == "/clear")
            {
                scrollBuffer = 0;
                textBuffer->clear();
            }

            else if (checkForCommand == "/connect" && connected)
            {
                textBuffer->pop_back();
                insertText("Already Connected, disconnect first");
            }
            
            if(checkForCommand == "/reconnect")
            {
                textBuffer->pop_back();
                connectServer(clientMap, graphics);
            }
        }
        
        if(textBuffer->size() > h - 2)  // we ignore the top and the bottom positions.
        {
            for(int i=1; i < h - 1; i++)     // The "-3" is to account for i+1+y potentially overflowwing.
            {                              // We start at i=1 ot account for the vector at position 0
                
                std::string tmpString;
                if(((int)textBuffer->size() - i - scrollBuffer) >= 0)
                {
                    tmpString = textBuffer->at((textBuffer->size())-i-scrollBuffer);
                }
                else
                {
                    break;
                }
                
                int stringLength = (int)tmpString.length();
                
                
                if(stringLength >= w-4)
                {
                    int size = stringLength/(w-4);
                    
                    for(x = 1; x < size; x++)
                        tmpString.insert((x*w)-6, "\n");
                }
                
                tmpString.insert(0, "%c");
                tmpString.insert(tmpString.length(), "%c");
                
                console->print(x+1, h-1-i, (const char*)&tmpString[0], TCOD_COLCTRL_2, TCOD_COLCTRL_STOP);
                
            }
            
        }
        
        else
        {
            for(int i=1; i < textBuffer->size()+1; i++)     // The "-3" is to account for i+1+y potentially overflowwing.
            {                              // We start at i=1 ot account for the vector at position 0
                
                
                std::string tmpString = textBuffer->at((textBuffer->size())-i);
                
                int stringLength = (int)tmpString.length();
                
                
                if(stringLength >= w)
                {
                    int size = stringLength/(w);
                    
                    for(x = 1; x < size; x++)
                        tmpString.insert((x*w)-6, "\n");
                }
                
                tmpString.insert(0, "%c");
                tmpString.insert(tmpString.length(), "%c");
                
                console->print(x+1, h-1-i, (const char*)&tmpString[0], TCOD_COLCTRL_2, TCOD_COLCTRL_STOP);
                
            }
        }
    }
    
    
}





/*
 
 Making note of this for my own memory, the purpose of having to specify
 the console at class initialization is so that the following functions
 work properly.
 
 */

void ScrollBox::onButtonPress()
{
    if(textBuffer->size() > h - 2)  // we ignore the top and the bottom positions.
    {
        if(onUpArrow)
        {
            if(scrollBuffer < maxBuffer && scrollBuffer < textBuffer->size() - h + 2)
                scrollBuffer++;
        }
        else if(onDownArrow)
        {
            if(scrollBuffer > 0)
                scrollBuffer--;
        }
    }
}


void ScrollBox::onButtonRelease()
{
    
}


void ScrollBox::onScrollUp()
{
    if(scrollBuffer < maxBuffer && scrollBuffer < textBuffer->size() - h + 2)
        scrollBuffer++;
}


void ScrollBox::onScrollDown()
{
    if(scrollBuffer > 0)
        scrollBuffer--;
}















