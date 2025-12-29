#include "../../headers/views/GameView.hpp"

GameView::GameView(SDL_Window *window, SDL_Renderer *renderer, SDLFontManager *fontManager, SDLTextureManager *textureManager, Board *board):
    chatBox(window, renderer, fontManager, {100,100,200,600}, "Roboto/Roboto-Medium", 18, true), selected(-1, -1)
{
    //init basic variables
    this->window = window;
    this->renderer = renderer;
    this->fontManager = fontManager;
    this->board = board;
    if (this->nullBoard = (this->board == nullptr)) this->board = new Board();

    //texture loading
    textures.emplace("chessboard", textureManager->getTexture("chessboard"));
    textures.emplace("valid_marker", textureManager->getTexture("valid_marker"));
    textures.emplace("pawn_black", textureManager->getTexture("pawn_black"));
    textures.emplace("pawn_white", textureManager->getTexture("pawn_white"));
    textures.emplace("rook_black", textureManager->getTexture("rook_black"));
    textures.emplace("rook_white", textureManager->getTexture("rook_white"));
    textures.emplace("queen_black", textureManager->getTexture("queen_black"));
    textures.emplace("queen_white", textureManager->getTexture("queen_white"));
    textures.emplace("knight_black", textureManager->getTexture("knight_black"));
    textures.emplace("knight_white", textureManager->getTexture("knight_white"));
    textures.emplace("king_black", textureManager->getTexture("king_black"));
    textures.emplace("king_white", textureManager->getTexture("king_white"));
    textures.emplace("bishop_black", textureManager->getTexture("bishop_black"));
    textures.emplace("bishop_white", textureManager->getTexture("bishop_white"));

    //add destination to textures
    destinationRectangles.emplace("chessboard", SDL_Rect{40, 40, 1024, 1024});
    destinationRectangles.emplace("piece_universal", SDL_Rect{0, 0, 128, 128});
    destinationRectangles.emplace("chat", SDL_Rect{1200, 40, 620, 920});
    destinationRectangles.emplace("chat_box", SDL_Rect{1200, 980, 620, 60});

    //Debug textures
    textures.emplace("piece_marker", textureManager->getTexture("piece_marker"));

    //ensure chat is cleared
    chat = "";
}

GameView::~GameView()
{
    if (nullBoard == true) delete this->board;
}

void GameView::render()
{
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);

    SDL_RenderClear(renderer);

    //render all textures in correct spaces
    SDL_RenderCopy(renderer, textures.at("chessboard"), nullptr, &destinationRectangles.at("chessboard"));
    // SDL_RenderCopy(renderer, , nullptr, &destinationRectangles.at("chat"));
    // SDL_RenderCopy(renderer, , nullptr, &destinationRectangles.at("chat_box"));
    

    //make it side dependent
    //here smart placing textures on board
    const std::vector<std::vector<char>> charBoard = board->getBoardSymbol();

    if (selected.first != -1 || selected.second != -1)
    {
        destinationRectangles.at("piece_universal") = {40 + (int)selected.second*128, 40 + (int)selected.first*128, 128, 128};
        SDL_RenderCopy(renderer, textures.at("piece_marker"), nullptr, &destinationRectangles.at("piece_universal"));
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            if (charBoard[i][j] == ' ') continue;

            destinationRectangles.at("piece_universal") = {40 + (int)j*128, 40 + (int)i*128, 128, 128};
            switch (charBoard[i][j])
            {
            case 'P':
                SDL_RenderCopy(renderer, textures.at("pawn_white"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'p':
                SDL_RenderCopy(renderer, textures.at("pawn_black"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'Q':
                SDL_RenderCopy(renderer, textures.at("queen_white"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'q':
                SDL_RenderCopy(renderer, textures.at("queen_black"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'K':
                SDL_RenderCopy(renderer, textures.at("king_white"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'k':
                SDL_RenderCopy(renderer, textures.at("king_black"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'R':
                SDL_RenderCopy(renderer, textures.at("rook_white"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'r':
                SDL_RenderCopy(renderer, textures.at("rook_black"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'B':
                SDL_RenderCopy(renderer, textures.at("bishop_white"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'b':
                SDL_RenderCopy(renderer, textures.at("bishop_black"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'N':
                SDL_RenderCopy(renderer, textures.at("knight_white"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            case 'n':
                SDL_RenderCopy(renderer, textures.at("knight_black"), nullptr, &destinationRectangles.at("piece_universal"));
                break;
            default:
                break;
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void GameView::handleEvent(const SDL_Event &e)
{
    //handle events in game e.g. playing it draging pices and other
    
}

void GameView::updateGame(Board *board)
{
    //maybe not needed

}

void GameView::updateChat(std::string& message, User& sender)
{
    std::string toAdd = sender.getUsername() + " " + std::to_string(sender.getPlayerID()) + ": " + message + "\n";
    std::string newChat = chat + sender.getUsername() + " " + std::to_string(sender.getPlayerID()) + ": " + message + "\n";
    if (newChat.size() > 256)
    {
        chat.erase(0, toAdd.size());
        if (size_t index = chat.find('\n') != 0)
        {
            chat.erase(0, index);
        }
    }
    else if (std::count(newChat.begin(), newChat.end(), '\n') > 30)
    {
        if (size_t index = chat.find('\n') != 0)
        {
            chat.erase(0, index);
        }
    }
    chat += sender.getUsername() + " " + std::to_string(sender.getPlayerID()) + ": " + message + "\n";
}

char GameView::checkPiece()
{
    if (selected.first == -1 || selected.second == -1) return ' ';
    return this->board->getBoardSymbol()[selected.first][selected.second];
}

std::pair<int, int> &GameView::getSelected()
{
    return this->selected;
}

void GameView::setSelected(int x, int y)
{
    this->selected = {x, y};
}

Board *GameView::getBoard()
{
    return this->board;
}
