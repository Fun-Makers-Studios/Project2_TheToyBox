#include "Fonts.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"

#include<string.h>
#include "Defs.h"
#include "Log.h"

Fonts::Fonts() : Module() 
{
	name.Create("Fonts_Module");
}

Fonts::~Fonts() 
{}

// Load new texture from file path
int Fonts::Load(const char* texture_path, const char* characters, uint rows) {
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0) {
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS) {
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS) {
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;



	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	font.totalLength = strlen(characters);
	font.columns = fonts[id].totalLength / rows;

	uint tex_w, tex_h;
	app->tex->GetSize(tex, tex_w, tex_h);
	font.char_w = tex_w / font.columns;
	font.char_h = tex_h / font.rows;

	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void Fonts::UnLoad(int font_id) {
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].texture != nullptr) {
		app->tex->UnLoad(fonts[font_id].texture);
		fonts[font_id].texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void Fonts::BlitText(int x, int y, int font_id, const char* text, bool useCamera) const {
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].texture == nullptr) {
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i) {

		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j) {
			if (font->table[j] == text[i]) {
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, SDL_FLIP_NONE, !useCamera);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}

int Fonts::BlitText2(int x, int y, int font_id, const char* text, int spacing, int lineLenght, bool useCamera) const {
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].texture == nullptr) {
		LOG("Unable to render text with bmp font id %d", font_id);
		return 0;
	}

	int lines = 1;

	const Font* font = &fonts[font_id];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	iPoint textpos = { x,y };

	for (uint i = 0; i < len; ++i) {

		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j) {
			if (font->table[j] == text[i]) {
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		if (text[i] == ' ') {
			if (spacing > -1 && lineLenght > 0)
			{
				int check = textpos.x;
				for (uint j = i + 1; text[j] != ' ' && j < len; ++j) {
					check += spriteRect.w;
				}

				if ((check - x) > lineLenght) {
					textpos.x = x;
					textpos.y += spriteRect.h + spacing;
					lines++;
					continue;
				}
			}
		}

		app->render->DrawTexture(font->texture, textpos.x, textpos.y, &spriteRect, SDL_FLIP_NONE, !useCamera);

		// Advance the position where we blit the next character
		textpos.x += spriteRect.w;
	}

	return lines;
}