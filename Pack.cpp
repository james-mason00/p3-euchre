#include "Pack.hpp"
#include <iostream>
#include <cassert>
using namespace std;

//default ctor
Pack::Pack() : next(0) {
    int i = 0;
    for(int s = SPADES; s <= DIAMONDS; ++s) {
        for(int r = NINE; r <= ACE; ++r) {
            //cast integer suit and rank to actual suit and rank
            Suit suit = static_cast<Suit>(s);
            Rank rank = static_cast<Rank>(r);
            //ctor
            Card c(rank, suit);
            //overwrite empty element
            cards.at(i) = c;
            ++i;    //increment such that we move to next index
        }
    }
}

//ctor from istream
Pack::Pack(istream &pack_input) : next(0) {
    for(int i = 0; i < PACK_SIZE; ++i) {
        Rank rank;
        string of;  //will be discarded
        Suit suit;
        //read in card
        pack_input >> rank >> of >> suit;
        //ctor
        Card c(rank, suit);
        //overwrite empty element
        cards.at(i) = c;
    }           
}

Card Pack::deal_one() {
    return cards.at(next);
    ++next;
}

void Pack::reset() {
    next = 0;
}