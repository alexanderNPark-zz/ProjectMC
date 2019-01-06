#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <time.h>

#define CARDS 52
#define SUITS 4
#define FACES 13
#define HAND_SIZE 5
#define MAX_TURNS 3
#define println printf("\n")

typedef struct {
    const char *face;
    const char *suit;
    int faceValue;
} Card; // end struct card

enum Status{
    Nothing,
    Pair,
    TwoPairs,
    Triple,
    Straight,
    Flush,
    FullHouse,
    FourKind,
    StraightFlush
};

void fillDeck(Card *const wDeck, const char *wFace[], const char *wSuit[]);

void shuffle( Card * const wDeck );

void deal(Card *const wDeck, int *deckPointer, Card *hand[]);
void ShowDeck(const Card * const wDeck);
int checkHand(Card *hand[], int minCase, int maxCase);
void printHand(const char *name,Card *hand[]);
void selectionSortHand(Card *hand[] );


int main() {

    const char *faces[] = { "Ace", "Deuce", "Three", "Four", "Five",
                            "Six", "Seven", "Eight", "Nine", "Ten",
                            "Jack", "Queen", "King"};

    // initialize array of pointers
    const char *suits[] = { "Hearts", "Diamonds", "Clubs", "Spades"};




    Card const deck[ CARDS ]; //

    srand( time( NULL ) ); // randomize

    fillDeck( deck, faces, suits );
    //ShowDeck(deck);
    println;
    //shuffle(deck);


    startGameSim(deck);





    return 0;
}

void startGameSim(Card const *deck){

    int deckPointer;
    char *name = "YOU";
    char *dealerName = "DEALER";
    Card *hand1[HAND_SIZE], *dealer[HAND_SIZE];


    int hand_status_dealer, hand_status_user;

    size_t counter;

    char user_choice[4];

    for(counter=0;counter<30; counter++){
        shuffle(deck);
        ShowDeck(deck);
        deckPointer=0;
        int stillPlay=1;
        int index_remove = -1;
        int turns=0;
        deal(deck,&deckPointer,hand1);
        deal(deck,&deckPointer,dealer);

        while(stillPlay && turns<MAX_TURNS){
            //selectionSortHand(hand1);
            printHand(name,hand1);
            hand_status_user = checkHand(hand1,Nothing,StraightFlush);

            printf("Swap a card out? type yes (any other means no):");
            scanf("%3s",user_choice);
            if(strcmp(user_choice,"yes")==0){
                index_remove = bruteForceCardReplace(deck, hand1,hand_status_user,deckPointer, -1,Nothing);
                hitAndRemove(deck,&deckPointer,index_remove,hand1);

                turns++;
            }else{
                stillPlay=0;
            }

            hand_status_dealer = checkHand(dealer,Nothing,StraightFlush);
            index_remove = bruteForceCardReplace(deck, dealer ,hand_status_dealer,deckPointer, StraightFlush-hand_status_dealer,hand_status_dealer);
            hitAndRemove(deck,&deckPointer,index_remove,hand1);
            println;

        }
        printHand(dealerName,dealer);
        if(hand_status_dealer==hand_status_user){
            printf("TIE GAME");
        }
        else{
            printf("%s",hand_status_dealer>hand_status_user? "DEALER WINS":"YOU WIN");
        }
        println;
    }

}

void hitAndRemove(Card *deck,int *deckPointer, int indexOfCardRemoved,Card *hand[]){
    if(indexOfCardRemoved==-1){
        return;
    }
    hand[indexOfCardRemoved]=&deck[(*deckPointer)++];
}

int bruteForceCardReplace(Card *deck, Card *hand[],int currentStatus, int deckPointer, int incOrDecr,int minCase){

    int duplicate;
    Card *holder;
    //Card *copy[HAND_SIZE];
    //for(int i=0;i<HAND_SIZE;i++) copy[i]= hand[i];

    double results[HAND_SIZE];
    int highestIndex=0;
    for(int i=0;i<HAND_SIZE;i++){
        holder = hand[i];
        for(duplicate = deckPointer; duplicate<CARDS;duplicate++){
            hand[i]=&deck[duplicate];
            int stat;
            if(incOrDecr>0){
                stat = checkHand(hand,minCase,currentStatus+incOrDecr);
            }else{
                stat = checkHand(hand,currentStatus+incOrDecr,minCase);
            }
            results[i]+=stat;
        }
        hand[i]=holder;
        results[i]/=(CARDS- deckPointer);
        if(results[i]>results[highestIndex]){
            highestIndex = i;
        }
    }

    return highestIndex;




}




int checkHand(Card *hand[], int minimumCase, int maxCase){
    //printf("Status: ");

    if(maxCase>=StraightFlush && minimumCase<=StraightFlush&& hasStraightFlush(hand)){
        //puts("STRAIGHT FLUSH");
        return StraightFlush;
    }
    else if(maxCase>=FourKind && minimumCase<=FourKind && hasFourCard(hand)){
        //puts("Four Of a kind found!");
        return FourKind;
    }
    if(maxCase>=FullHouse&& minimumCase<=FullHouse && hasHouse(hand)){
        //puts("Full House found!");
        return FullHouse;
    }
    else if(maxCase>=Flush && minimumCase<=Flush && hasFlush(hand)){
        //puts("Flush found!");
        return Flush;
    }
    else if(maxCase>=Straight && minimumCase<=Straight && hasStrait(hand)) {
        //puts("Straight found!");
        return Straight;
    }
    else if(maxCase>=Triple && minimumCase<=Triple && hasTriple(hand)){
        //puts("Triple found!");
        return Triple;
    }
    else if(maxCase>=TwoPairs&& minimumCase<=TwoPairs && hasTwoPairs(hand)){
        //puts("Two Pairs found!");
        return TwoPairs;
    }
    else if(maxCase>=Pair && minimumCase<=Pair && hasPair(hand)){
        //puts("Pair found!");
        return Pair;
    }

    else{
        //puts("Nothing!");
        return Nothing;
    }
    println;

}

void fillDeck(Card *const wDeck, const char *wFace[], const char *wSuit[]) {
    size_t i; // counter

// loop through wDeck
    for (i = 0; i < CARDS; ++i) {
        wDeck[ i ].face = wFace[ i % FACES ];
        wDeck[ i ].suit = wSuit[ i / FACES ];
        wDeck[i].faceValue = i % FACES;

    } // end for
} // end function fillDeck

void shuffle(Card *const wDeck){

    size_t i; // counter
    size_t j; // variable to hold random value between 0 - 51
    Card temp; // define temporary structure for swapping Cards

    // for each of the cards, choose slot of deck randomly
    for ( i = 0; i < CARDS; ++i ) {

        j = rand() % CARDS;
        temp = wDeck[i];
        wDeck[i] = wDeck[j];
        wDeck[j] = temp;

    } // end for
} // end function shuffle

void deal(Card *const wDeck, int *deckPointer, Card *hand[]){
    int i;
    for(i=0;i<HAND_SIZE;i++){
        hand[i]=&wDeck[*deckPointer];
        (*deckPointer)++;
    }

}

void ShowDeck(const Card * const wDeck)
{
    size_t i; // counter

    // loop through wDeck
    for ( i = 0; i < CARDS; ++i ) {
        printf( "%6s of %-12s%s",wDeck[ i ].face ,wDeck[ i ].suit,
                ( i + 1 ) % 4 ? " " : "\n" );
    } // end for
} // end function deal

void printHand(const char *name,Card *hand[]){
    size_t i;
    printf("%s:", name);
    for(i=0;i<HAND_SIZE;i++){
        printf("%4d:%6s of %-8s",i,hand[i]->face,hand[i]->suit);
    }
    println;
}
/*
int compareCardValues(Card *card1, Card* card2, const char *faces[]){
    int face1,face2=0;
    size_t i;
    for(i=0;i<FACES;i++){
        if(strcmp(card1->face,faces[i])==0){
            face1=i+1;
        }
        if(strcmp(card2->face,faces[i])==0){
            face2=i+1;
        }
    }
    if(face1==face2){
        return 0;
    }
    return face1>face2? 1 : -1;
}
*/
void selectionSortHand(Card *hand[] ){
    size_t i,j;
    int smallest;

    for(i=0;i<HAND_SIZE;i++){
        smallest = i;
        for(j=i+1;j<HAND_SIZE;j++){
            if(hand[j]->faceValue<hand[smallest]->faceValue){
                smallest = j;
            }
        }
        Card *temp = hand[smallest];
        hand[smallest]=hand[i];
        hand[i] = temp;

    }
}


int hasFlush(Card *hand[]){
    const char *saved_suit = hand[0]->suit;
    int i;
    for(i=1;i<HAND_SIZE;i++){
        if(strcmp(saved_suit,hand[i]->suit)!=0){
            return 0;
        }
    }
    return 1;
}


int hasPair(Card *hand[]){
    return hasMatching(hand,2)==1;
}

int hasTwoPairs(Card *hand[]){
    return hasMatching(hand,2)==2;
}

int hasTriple(Card *hand[]){
    return hasMatching(hand,3);
}

int hasFourCard(Card *hand[]){
    return hasMatching(hand,4);
}

int hasHouse(Card *hand[]){
    return hasPair(hand) && hasTriple(hand);
}

int hasMatching(Card *hand[], int matchingFactor){
    /*
     * Generic matching method.
     * This was designed to be reused by all cards that require matching
     */
    int numOfEach[FACES]={0};
    int i =0;
    for(i=0;i<HAND_SIZE;i++){
        numOfEach[hand[i]->faceValue]++;
    }
    int numberOfMatches=0;
    for(i=0;i<FACES;i++){
        if(numOfEach[i]==matchingFactor){
            numberOfMatches++;
        }
    }
    return numberOfMatches;
}

int hasStraightFlush(Card *hand[]){
    return hasStrait(hand) && hasFlush(hand);
}

int hasStrait(Card *hand_real[]){
    Card *hand[HAND_SIZE];
    for(int i=0;i<HAND_SIZE;i++) hand[i]= hand_real[i];
    selectionSortHand(hand);
    int prev=hand[0]->faceValue;
    size_t i;
    for(i=1;i<HAND_SIZE;i++){
        if(prev+1!=hand[i]->faceValue){
            return 0;
        }
        prev = hand[i]->faceValue;
    }
}




