/*

  Our Window Functions

 */

#include "Window.h"

#include "SDL/SDL.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 748;
const int SCREEN_BPP = 32;





Window::Window(){

  // Set up the screen
  screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE );

  if( screen == NULL ){
    windowOK = false;
    return;
  }
  else{
    windowOK = true;
  }
  
  SDL_WM_SetCaption( "The ASCII Project - Client - 0.0.1f", NULL);

  windowed = true;
  
}


void Window::Toggle_Fullscreen(){
  
  //If the screen is windowed
  if( windowed == true )
    {
      //Set the screen to fullscreen
      screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN );
      
      //If there's an error
      if( screen == NULL )
        {
          windowOK = false;
          return;
        }
      
      //Set the window state flag
      windowed = false;
    }
  //If the screen is fullscreen
  else if( windowed == false )
    {
      //Window the screen
      screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE );
      
      //If there's an error
      if( screen == NULL )
        {
          windowOK = false;
          return;
        }
      
      //Set the window state flag
      windowed = true;
    }
}



void Window::Handle_Events(SDL_Event event){

//If there's something wrong with the window
    if( windowOK == false )
    {
        return;    
    }
    
    //If the window resized
    if( event.type == SDL_VIDEORESIZE )
    {
        //Resize the screen
        screen = SDL_SetVideoMode( event.resize.w, event.resize.h, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE );
        
        //If there's an error
        if( screen == NULL )
        {
            windowOK = false;
            return;
        }
    }

 //If enter was pressed
    else if( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_RETURN ) )
    {
        //Turn fullscreen on/off
        Toggle_Fullscreen();
    }


 //If the window focus changed
    else if( event.type == SDL_ACTIVEEVENT )
    {
        //If the window was iconified or restored
        if( event.active.state & SDL_APPACTIVE )
        {
            //If the application is no longer active
            if( event.active.gain == 0 )
            {
                SDL_WM_SetCaption( "Window Event Test: Iconified", NULL );
            }
            else
            {
                SDL_WM_SetCaption( "Window Event Test", NULL );
            }
        }


//If something happened to the keyboard focus
        else if( event.active.state & SDL_APPINPUTFOCUS )
        {
            //If the application lost keyboard focus
            if( event.active.gain == 0 )
            {
                SDL_WM_SetCaption( "Window Event Test: Keyboard focus lost", NULL );
            }
            else
            {
                SDL_WM_SetCaption( "Window Event Test", NULL );
            }
        }
        //If something happened to the mouse focus
        else if( event.active.state & SDL_APPMOUSEFOCUS )
        {
            //If the application lost mouse focus
            if( event.active.gain == 0 )
            {
                SDL_WM_SetCaption( "Window Event Test: Mouse Focus Lost", NULL );
            }
            else
            {
                SDL_WM_SetCaption( "Window Event Test", NULL );
            }
        }
    }
//If the window's screen has been altered
    else if( event.type == SDL_VIDEOEXPOSE )
    {
        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            //If there's an error
            windowOK = false;
            return;
        }
    }
}


bool Window::Error()
{
    return !windowOK;
}
