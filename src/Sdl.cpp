#include <SDL/SDL.h>
#include <pthread.h>

#include "graphics/Nv2a.h"
#include "Log.h"
#include "Mem.h"
#include "Sdl.h"
#include "Types.h"

pthread_t SdlThread;

static void* SDLUpdateThread()
{
	SDL_Surface *screen = NULL;

	SDL_Init( SDL_INIT_EVERYTHING );

	screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
	SDL_WM_SetCaption( "XBVM", "XBVM" );
	for(;;) {
		u32* pixels = (u32*)screen->pixels;
		if( g_nv2a->IsVideoOverlayValid() ) {
			u32 overlayAddress = g_nv2a->GetOverlayAddress();
			for( int i = 0; i < 640 * 480; i++ ) {
				pixels[i] = g_mem->ReadPhysU32( overlayAddress + (i * 4) );
			}
		}
		else {
			for( int i = 0; i < 640 * 480; i++ ) {
				pixels[i] = 0;
			}
		}
		SDL_Flip( screen );
		SDL_Delay( 16 );
	}
	return 0;
}

void StartSdlThread()
{
	pthread_create( &SdlThread, NULL, (void*(*)(void*))SDLUpdateThread, NULL );
}

