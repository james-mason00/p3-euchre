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
    Card c = cards.at(next);
    ++next;
    return c;
}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle() {
    for(int i = 0; i < 7; ++i) {
        array<Card, PACK_SIZE> temp = cards;
        //holy brute force lol
        cards.at(0) = temp.at(12);
        cards.at(1) = temp.at(0);
        cards.at(2) = temp.at(13);
        cards.at(3) = temp.at(1);
        cards.at(4) = temp.at(14);
        cards.at(5) = temp.at(2);
        cards.at(6) = temp.at(15);
        cards.at(7) = temp.at(3);
        cards.at(8) = temp.at(16);
        cards.at(9) = temp.at(4);
        cards.at(10) = temp.at(17);
        cards.at(11) = temp.at(5);
        cards.at(12) = temp.at(18);
        cards.at(13) = temp.at(6);
        cards.at(14) = temp.at(19);
        cards.at(15) = temp.at(7);
        cards.at(16) = temp.at(20);
        cards.at(17) = temp.at(8);
        cards.at(18) = temp.at(21);
        cards.at(19) = temp.at(9);
        cards.at(20) = temp.at(22);
        cards.at(21) = temp.at(10);
        cards.at(22) = temp.at(23);
        cards.at(23) = temp.at(11);
    }
    next = 0;
}

bool Pack::empty() const {
    return next == PACK_SIZE;
}