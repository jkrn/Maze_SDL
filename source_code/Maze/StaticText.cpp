#include "StaticText.hpp"
#include "Constants.hpp"
#include <string>

StaticText::StaticText(TTF_Font* font, std::string text, SDL_Color textColor, SDL_Rect* textPos, int textWidthInPixel, SDL_Renderer* renderer) :
	font(font),
	textColor(textColor),
	textWidthInPixel(textWidthInPixel),
	renderer(renderer)
{
	if (textWidthInPixel > 0) {
		// Create Surface
		SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), whiteColor, textWidthInPixel);
		// Create Texture
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_SetTextureColorMod(textTexture, textColor.r, textColor.g, textColor.b);
		// Set position
		if (textPos == NULL) {
			this->textPos = { 0, 0, textSurface->w, textSurface->h };
		}
		else {
			this->textPos = { textPos->x, textPos->y, textSurface->w, textSurface->h };
		}
		// Free surface
		SDL_FreeSurface(textSurface);
	}
}

StaticText::~StaticText() {
	// Release Texture
	SDL_DestroyTexture(textTexture);
}

void StaticText::showText() {
	SDL_RenderCopy(renderer, textTexture, NULL, &textPos);
}

void StaticText::updateText(std::string text) {
	SDL_DestroyTexture(textTexture);
	textTexture = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), statusTextColor, textWidthInPixel);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_SetTextureColorMod(textTexture, textColor.r, textColor.g, textColor.b);
	textPos = { textPos.x, textPos.y, textSurface->w, textSurface->h };
	SDL_FreeSurface(textSurface);
}

void StaticText::updateColor(SDL_Color textColor) {
	this->textColor = textColor;
	SDL_SetTextureColorMod(textTexture, textColor.r, textColor.g, textColor.b);
}