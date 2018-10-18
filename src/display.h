#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <string>
#include <SDL.h>

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void SwapBuffers();

	virtual ~Display();
	inline SDL_Window* getWindow() { return this->m_window; }
	inline SDL_GLContext getContext() { return this->m_glContext;  }

protected:
private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
};

#endif
