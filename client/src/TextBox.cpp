#include "../headers/TextBox.hpp"
#include "TextBox.hpp"



TextBox::TextBox(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, const SDL_Rect& pos, 
        const std::string& font, int size, SDL_Color color) : boxPos(pos), font(font), color(color), fontSize(size)   
{
    //set data
    this->window = window;
    this->renderer = renderer;
    this->fontManager = fontManager;
    this->texture = nullptr;
    this->textureSize = {pos.x, pos.y, 0, 0};
    this->text = "";
    render = false;

    //create needed data
    this->fontManager->addNewFont(font, size);
}

TextBox::~TextBox()
{
    SDL_DestroyTexture(this->texture);
}

bool TextBox::checkIfClicked(int x, int y)
{
    return boxPos.x <= x && (boxPos.x + boxPos.w) >= x && boxPos.y <= y && (boxPos.y + boxPos.h) >= y;
}

void TextBox::genTexture()
{
    if (this->render)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = this->fontManager->createTextTexture(this->text, this->font, this->fontSize, this->color);
        SDL_QueryTexture(this->texture, nullptr, nullptr, &this->textureSize.w, &this->textureSize.h);
    }
}

SDL_Texture *TextBox::getTexture()
{
    return this->texture;
}

SDL_Rect& TextBox::getTextureRect()
{
    return this->textureSize;
}

void TextBox::setText(const std::string &text)
{
    this->text = text;
    this->render = true;
}

void TextBox::textListener(SDL_Event &e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
        this->handleSpecialKeyPress(e);
        break;
    case SDL_TEXTINPUT:
        if (!((SDL_GetModState() & KMOD_CTRL) && 
            (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
        {
            this->text += e.text.text;
            this->render = true;
        }
    default:
        break;
    }
}

std::string &TextBox::getString()
{
    return this->text;
}

void TextBox::handleSpecialKeyPress(SDL_Event &e)
{
    switch (e.key.keysym.sym)
    {
    case SDLK_BACKSPACE:
        if (this->text.size() > 0 && !(SDL_GetModState() & KMOD_CTRL))
        {
            this->text.pop_back();
            this->render = true;
        }
        else if (this->text.size() > 0)
        {
            this->text = "";
            this->render = true;
        }
        break;
    case SDLK_c:
        if (SDL_GetModState() & KMOD_CTRL)
        {
            SDL_SetClipboardText(this->text.c_str());
        }
        break;
    case SDLK_v:
        if (SDL_GetModState() & KMOD_CTRL)
        {
            char* text = SDL_GetClipboardText();
            this->text.append(text);
            SDL_free(text);
            this->render = true;
        }
        break;
    default:
        break;
    }
}