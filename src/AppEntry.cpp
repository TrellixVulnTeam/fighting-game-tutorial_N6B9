#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

SDL_Window* sdlWindow = nullptr;
SDL_GLContext glContext = nullptr;

void update_game_state()
{

}

void render_frame()
{
	// Clear render target
	glClearColor(0.f, 0.5f, 1.f, 1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Present drawn image to the window surface
	SDL_GL_SwapWindow(sdlWindow);
}

int main()
{
	// Initialize SDL library
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Failed to initialize SDL library!" << std::endl;
		return 1;
	}

	// Create non-resizable window with SDL support
	sdlWindow = SDL_CreateWindow("Fighting Game", 50, 50, 1600, 900, SDL_WINDOW_OPENGL);
	if (sdlWindow)
	{
		// Create OpenGL context for rendering
		glContext = SDL_GL_CreateContext(sdlWindow);
		if (!glContext)
		{
			std::cerr << "Failed to create OpenGL context!" << std::endl;
			return 1;
		}
		else
		{
			// Apply create OpenGL context and initialize OpenGL extensions using glew
			SDL_GL_MakeCurrent(sdlWindow, glContext);
			glewInit();
		}
	}

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
		update_game_state();

		// Perform rendering of the frame according to new state
		render_frame();
	}

	// Delete OpenGL context
	if (glContext)
	{
		SDL_GL_DeleteContext(glContext);
	}

	// Shutdown SDL library
	SDL_Quit();

	return 0;
}
