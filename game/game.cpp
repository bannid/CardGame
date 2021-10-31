#include "game.h"



int GetNumberOfFlippedCards(Game * game);
int GetNumberOfMatchedCards(Game * game);
bool AnyOtherMatchedCardAnimating(Game * game);
void RenderGame(Game * game, float timeDelta);
void PlaySound(PlaySoundCallback * callback, SoundType type);
DLL_API void UpdateGame(Game * game, float elapsedTime){
    if(game->currentLevel->isWon)return;
    int totalNumberOfCards = game->currentLevel->totalNumberOfCards;
    nGame::Card * cards = game->currentLevel->cards; 
    if(GetNumberOfMatchedCards(game) == totalNumberOfCards){
        PlaySound(game->playSoundCallback, GAME_WON);
        game->currentLevel->isWon = true;
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
    if(game->state != PLAYING) return;
    game->currentLevel->elapsedTime += elapsedTime;
    RenderGame(game, elapsedTime);
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
        if(totalFlippedCards == 2){
            nGame::Card * firstCard = NULL;
            nGame::Card * secondCard = NULL;
            int counter = 0;
            for(int i = 0; i<totalNumberOfCards; i++){
                nGame::Card * card = cards + i;
                //Refactor
                if(card->isFlipped && !card->isMatched){
                    if(counter == 0){
                        firstCard = card;
                        counter++;
                    }
                    else {
                        secondCard = card;
                        counter++;
                    }
                    
                }
            }
            if(firstCard->suit == secondCard->suit && 
            firstCard->rank == secondCard->rank){
                firstCard->isMatched = true;
                secondCard->isMatched = true;
            }
        }
    //We check if the card clicked was flipped.
    if(cardClicked != NULL && cardClicked->isFlipped){
        //We check if there are more than 2 cards flipped.
        if(totalFlippedCards > 2){
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
    }
}

void RenderGame(Game * game, float timeDelta){
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
        game->drawObject3D(game->front, game->shader, game->vao);
        Texture t;
        if(game->getTextureTextureManager(game->textureManager, "CardBack", &t)){
            game->textureApi.attachCallback(&t);
        }
        game->drawObject3D(game->back, game->shader, game->vao);
    }
}

int GetNumberOfFlippedCards(Game * game){
    int number = 0;
    for(int i = 0; i<game->currentLevel->totalNumberOfCards; i++){
        if(game->currentLevel->cards[i].isFlipped && !game->currentLevel->cards[i].isMatched){
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
        case GAME_WON:{
            callback(SOUND_FILE(breakout.mp3), false);
            break;
        }
    }
}