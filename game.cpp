#include "game.h"

void UpdateGame(Game * game){
    int totalNumberOfCards = game->currentLevel->totalNumberOfCards;
    Card * cards = game->currentLevel->cards;
    //Rotate all the cards that shouldn't be flipped
    for(int i = 0; i<totalNumberOfCards; i++){
        Card * card = cards + i;
        if(card->shouldntBeFlipped && !card->rotateAnimation.isActive){
            ClickCard(card);
            card->shouldntBeFlipped = false;
        }
    }
    if(game->state != PLAYING) return;
    Card * cardClicked = NULL;
    //If the left mouse button was clicked
    if(Input::MouseKeyWasReleased(GLFW_MOUSE_BUTTON_LEFT)){
        double x, y;
        Input::GetMousePositions(&x, &y);
        OpenglCoords coordsOpengl = ScreenToOpenglCoords(x, y, game->globalInfo);
        for(int i = 0; i<totalNumberOfCards; i++){
            Card * card = cards + i;
            if(CardWasClicked(card, coordsOpengl)){
                ClickCard(card);
                cardClicked = card;
            }
        }
    }
    //We check if the card clicked was flipped.
    if(cardClicked != NULL && cardClicked->isFlipped){
        //We check if there are more than 2 cards flipped.
        int totalFlippedCards = GetNumberOfFlippedCards(game);
        if(totalFlippedCards == 2){
            Card * firstCard = NULL;
            Card * secondCard = NULL;
            int counter = 0;
            for(int i = 0; i<totalNumberOfCards; i++){
                Card * card = cards + i;
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
        else if(totalFlippedCards > 2){
            //If there are more than two flipped cards,
            //we want to flip back all the cards that were flipped
            //previously.
            for(int i = 0; i<totalNumberOfCards; i++){
                Card * card = cards + i;
                if( card != cardClicked && card->isFlipped && !card->isMatched){
                    //If the previous card is still animating,
                    //set a flag to make sure the card will be flipped  back.
                    if(card->rotateAnimation.isActive){
                        card->shouldntBeFlipped = true;
                    }
                    //Otherwise just click the card
                    else{
                        ClickCard(card);
                    }
                }
            }
        }
    }
}

int GetNumberOfFlippedCards(Game * game){
    int number = 0;
    for(int i = 0; i<game->currentLevel->totalNumberOfCards; i++){
        if(game->currentLevel->cards[i].isFlipped && game->currentLevel->cards[i].isMatched){
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