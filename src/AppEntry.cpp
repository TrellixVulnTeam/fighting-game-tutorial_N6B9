#include <iostream>
#include <SDL2/SDL.h>

int main()
{
	// Initialize SDL library
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Failed to initialize SDL library!" << std::endl;
		return 1;
	}

	// Create non-resizable window with SDL support
	SDL_Window* sdlWindow = SDL_CreateWindow("Fighting Game", 50, 50, 1600, 900, SDL_WINDOW_OPENGL);

	// Enter SDL event processing loop
	bool shutdownRequested = false;
	while (!shutdownRequested)
	{
		// Process SDL events queue
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_QUIT:
					shutdownRequested = true;
					break;
			}
		}

		// Run application logic for this frame
	}

	// Shutdown SDL library
	SDL_Quit();

	return 0;
}
