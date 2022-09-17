#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class StaticText {
public:
	StaticText(TTF_Font* font, std::string text, SDL_Color textColor, SDL_Rect* textPos, int textWidthInPixel, SDL_Renderer* renderer);
	~StaticText();
	void showText();
	void updateText(std::string text);
	void updateColor(SDL_Color textColor);
private:
	TTF_Font * font;
	SDL_Color textColor;
	SDL_Rect textPos;
	int textWidthInPixel;
	SDL_Renderer* renderer;
	SDL_Texture* textTexture;
};