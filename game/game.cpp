#include "game.h"

int GetNumberOfFlippedCards(Game * game);
int GetNumberOfMatchedCards(Game * game);
bool AnyOtherMatchedCardAnimating(Game * game);
inline void RenderGame(Game * game, float timeDelta, float timeSinceStarting);
void PlaySound(PlaySoundCallback * callback, SoundType type);

DLL_API void UpdateGame(Game * game, float elapsedTime, float timeSinceStarting){
    int totalNumberOfCards = game->currentLevel->totalNumberOfCards;
    nGame::Card * cards = game->currentLevel->cards; 
    if(!game->currentLevel->isWon){
        if(GetNumberOfMatchedCards(game) == totalNumberOfCards && !AnyOtherMatchedCardAnimating(game)){
            game->currentLevel->isWon = true;
            game->state = GameState::GAME_WON;
            return;
        } 
        //Rotate all the cards that shouldn't be flipped
        for(int i = 0; i<totalNumberOfCards; i++){
            nGame::Card * card = cards + i;
            if(card->shouldntBeFlipped && !card->rotateAnimation.isActive){
                nGame::ClickCard(card);
                PlaySound(game->playSoundCallback, CARD_CLICK);
                card->shouldntBeFlipped = false;
            }
        }
    }
    switch(game->state){
        case PLAYING:{
            std::string p("Time left: ");
            int timeLeft = game->currentLevel->totalTime - game->currentLevel->elapsedTime;
            std::string s = std::to_string(timeLeft);
            game->renderTextCallback(p, game->characterSet, 20, TextAlign::ALIGN_RIGHT, glm::vec2(360, 200));
            game->renderTextCallback(s, game->characterSet, 20, TextAlign::ALIGN_RIGHT, glm::vec2(390, 200));
            game->currentLevel->elapsedTime += elapsedTime;
            RenderGame(game, elapsedTime, timeSinceStarting);
            nGame::Card * cardClicked = NULL;
            //If the left mouse button was clicked
            if(game->mouseKeyIsDownCallback(GLFW_MOUSE_BUTTON_LEFT)){
                double x, y;
                game->mousePositionCallback(&x, &y);
                OpenglCoords coordsOpengl = ScreenToOpenglCoords(x, y, game->globalInfo);
                for(int i = 0; i<totalNumberOfCards; i++){
                    nGame::Card * card = cards + i;
                    if(nGame::CardWasClicked(card, coordsOpengl)){
                        if(!card->rotateAnimation.isActive && !card->isMatched){
                            nGame::ClickCard(card);
                            PlaySound(game->playSoundCallback, CARD_CLICK);
                            cardClicked = card;
                        }
                        else{
                            card->clickCounter++;
                        }
                    }
                }
            }
            int totalFlippedCards = GetNumberOfFlippedCards(game);
            if(cardClicked != NULL && cardClicked->isFlipped && totalFlippedCards > 2){
                //If there are more than two flipped cards,
                //we want to flip back all the cards that were flipped
                //previously.
                for(int i = 0; i<totalNumberOfCards; i++){
                    nGame::Card * card = cards + i;
                    if( card != cardClicked && card->isFlipped && !card->isMatched){
                        //If the previous card is still animating,
                        //set a flag to make sure the card will be flipped  back.
                        if(card->rotateAnimation.isActive){
                            card->shouldntBeFlipped = true;
                        }
                        //Otherwise just click the card
                        else{
                            nGame::ClickCard(card);
                            PlaySound(game->playSoundCallback, CARD_CLICK);
                        }
                    }
                }
            }
            if(totalFlippedCards == 2){
                nGame::Card * firstCard = NULL;
                nGame::Card * secondCard = NULL;
                for(int i = 0; i<totalNumberOfCards; i++){
                    nGame::Card * card = cards + i;
                    if(card->isFlipped && !card->isMatched){
                        if(firstCard == NULL) firstCard = card;
                        else secondCard = card;
                    }
                }
                if(firstCard->suit == secondCard->suit && firstCard->rank == secondCard->rank){
                    firstCard->isMatched = true; 
                    secondCard->isMatched = true;
                }
            }
            if(game->currentLevel->elapsedTime > game->currentLevel->totalTime){
                game->state = GAME_OVER;
            }
            break;
        }
        
        case GAME_OVER:{
            game->renderTextCallback("Game over", game->characterSet, 50, TextAlign::ALIGN_CENTER, glm::vec2(200, 200));
            game->currentLevel->gameOverScreenTimeElapsed += elapsedTime;
            if(game->currentLevel->gameOverScreenTime < game->currentLevel->gameOverScreenTimeElapsed){
                game->state = GameState::STARTMENU;
            }
            break;
        }
        case GameState::GAME_WON:{
            game->renderTextCallback("You won", game->characterSet, 50, TextAlign::ALIGN_CENTER, glm::vec2(200, 200));
            game->currentLevel->gameOverScreenTimeElapsed += elapsedTime;
            if(game->currentLevel->gameOverScreenTime < game->currentLevel->gameOverScreenTimeElapsed){
                game->state = GameState::STARTMENU;
            }
            break;
        }
    }
}

DLL_API void InitGame(Game * game){
    game->loadShadersShaderManagerCallback(game->shaderManager, {
                       { "BackGroundShader2", "../shaders/backgroundShader.vert", "../shaders/backgroundShader.frag" }
                   });
}

inline void RenderGame(Game * game, float timeDelta, float timeSinceStarting){
    for(int i = 0; i < game->currentLevel->totalNumberOfCards; i++){
        nGame::Card * card = game->currentLevel->cards + i;
        if(card->isGone) continue;
        if((card->isMatched && !AnyOtherMatchedCardAnimating(game))){
            card->isGone = true;
            PlaySound(game->playSoundCallback, CARD_MATCH);
            continue;
        }
        Anim::UpdateRotateAnimationState(&card->rotateAnimation, timeDelta);
        //Set the positions and scales.
        game->front->scale = card->scale;
        game->back->scale = card->scale;
        game->front->position = card->position;
        game->back->position = card->position;
        game->back->rotation = card->rotateAnimation.currentValue;
        game->front->rotation = card->rotateAnimation.currentValue + 180.0f;
        
        Anim::UpdateRotateAnimation(&card->rotateAnimation);
        //After updating the animation, set the rotation
        game->back->rotation = card->rotateAnimation.currentValue;
        game->front->rotation = card->rotateAnimation.currentValue + 180.0f;
        if(card->clickCounter > 0 && !card->rotateAnimation.isActive && !card->isMatched){
            nGame::ClickCard(card);
            PlaySound(game->playSoundCallback, CARD_CLICK);
            card->clickCounter = 0;
        }
        game->textureApi.attachCallback((game->textures + card->rank + card->suit * 13));
        Shader shader; 
        game->getShaderShaderManagerCallback(game->shaderManager, "GameShader", &shader);
        game->drawQuadCallback(game->front, &shader, game->vao);
        Texture t;
        if(game->getTextureTextureManager(game->textureManager, "CardBack", &t)){
            game->textureApi.attachCallback(&t);
        }
        game->drawQuadCallback(game->back, &shader, game->vao);
    }           
}

int GetNumberOfFlippedCards(Game * game){
    int number = 0;
    for(int i = 0; i<game->currentLevel->totalNumberOfCards; i++){
        if(!game->currentLevel->cards[i].shouldntBeFlipped && game->currentLevel->cards[i].isFlipped && !game->currentLevel->cards[i].isMatched){
            number++;
        }
    }
    return number;
}

int GetNumberOfMatchedCards(Game * game){
    int number = 0;
    for(int i = 0; i<game->currentLevel->totalNumberOfCards; i++){
        if(game->currentLevel->cards[i].isMatched){
            number++;
        }
    }
    return number;
}

bool AnyOtherMatchedCardAnimating(Game * game){
    int number = 0;
    for(int i = 0; i<game->currentLevel->totalNumberOfCards; i++){
        nGame::Card * card = game->currentLevel->cards + i;
        if(card->isMatched && card->rotateAnimation.isActive){
            number++;
        }
    }
    return number;
}

void PlaySound(PlaySoundCallback * callback, SoundType type){
    switch(type){
        case CARD_CLICK:{
             callback(SOUND_FILE(solid.wav), false); 
            break;
        }
        case CARD_MATCH:{
            callback(SOUND_FILE(powerup.wav), false);
            break;
        }
        case SOUND_GAME_WON:{
            callback(SOUND_FILE(breakout.mp3), false);
            break;
        }
    }
}