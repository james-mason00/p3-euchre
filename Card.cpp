#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  return {};
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  return {};
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////


// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator>>
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=

//default ctor - 2 of spades
Card::Card()
: rank(TWO), suit(SPADES) {}

//custom ctor - specify rank and suit
Card::Card(Rank rank_in, Suit suit_in)
: rank(rank_in), suit(suit_in) {}

Rank Card::get_rank() const {
  return rank;
}

Suit Card::get_suit() const {
  return suit;
}

Suit Card::get_suit(Suit trump) const {
  //left bower considered a part of the trump suit
  if(is_left_bower(trump)) {
    return trump;
  }
  return suit;
}

bool Card::is_face_or_ace() const {
  if(get_rank() == JACK || get_rank() == QUEEN || get_rank() == KING || get_rank() == ACE) {
    return true;
  }
  return false;
}

//right bower is jack with same suit as trump
bool Card::is_right_bower(Suit trump) const {
  if(get_rank() == JACK && get_suit() == trump) {
    return true;
  }
  return false;
}

//left bower is jack of same color suit as trump
bool Card::is_left_bower(Suit trump) const {
  Suit next_suit = Suit_next(trump);
  if(get_suit() == next_suit && get_rank() == JACK) {
    return true;
  }
  return false;
}

//if suit matches trump or card is a left bower it is trump
bool Card::is_trump(Suit trump) const {
  if(get_suit() == trump || is_left_bower(trump)) {
    return true;
  }
  return false;
}

//note here we index into the suit/rank names vector for lowercase
std::ostream & operator<<(std::ostream &os, const Card &card) {
  os << RANK_NAMES[card.get_rank()] << " of " << SUIT_NAMES[card.get_suit()];
  return os;
}

std::istream & operator>>(std::istream &is, Card &card) {
  std::string rank_string;
  std::string of_string;    //will be discarded
  std::string suit_string;

  is >> rank_string >> of_string >> suit_string;

  //allowed access to private card variables through friend declaration!
  card.rank = string_to_rank(rank_string);
  card.suit = string_to_suit(suit_string);
  return is;
}

//returns true if lhs is less than than rhs 
//doesn't consider trump
bool operator<(const Card &lhs, const Card &rhs) {
  //tie breaking 
  if(lhs.get_rank() == rhs.get_rank()) {
    return lhs.get_suit() < rhs.get_suit();
  }
  return lhs.get_rank() < rhs.get_rank();
}

//returns true if lhs less than or equal to rhs
//doesn't consider trump
bool operator<=(const Card &lhs, const Card &rhs) {
  if(lhs.get_rank() == rhs.get_rank()) {
    return lhs.get_suit() <= rhs.get_suit();
  }
  return (lhs.get_rank() <= rhs.get_rank() || lhs == rhs);
}

//returns true if lhs greater than rhs 
//doesn't consider trump
bool operator>(const Card &lhs, const Card &rhs) {
  if(lhs.get_rank() ==  rhs.get_rank()) {
    return lhs.get_suit() > rhs.get_suit();
  }
  return lhs.get_rank() > rhs.get_rank();
}

//returns true if lhs rank greater than or equal to rhs rank
//doesn't consider trump
bool operator>=(const Card &lhs, const Card &rhs) {
  if(lhs.get_rank() == rhs.get_rank()) {
    return lhs.get_suit() >= rhs.get_suit();
  }
  return (lhs.get_rank() >= rhs.get_rank() || lhs == rhs);
}

//returns true if lhs and rhs are same card
//doesn't consider trump
bool operator==(const Card &lhs, const Card &rhs) {
  return (lhs.get_rank() == rhs.get_rank() && lhs.get_suit() ==  rhs.get_suit());
}

//returns true if lhs rank doesn't equal rhs rank
//doesn't consider trump
bool operator!=(const Card &lhs, const Card &rhs) {
  return !(lhs == rhs);
}

//next suit is other suit of same color
Suit Suit_next(Suit suit) {
  Suit next_suit;
  if(suit == DIAMONDS) {
    next_suit = HEARTS;
  }
  if(suit == HEARTS) {
    next_suit = DIAMONDS;
  }
  if(suit == SPADES) {
    next_suit = CLUBS;
  }
  if(suit == CLUBS) {
    next_suit = SPADES;
  }
  return next_suit;
}

/* 
when a trump is present, all trump cards > non-trump cards
right bower: most valuable card
left bower: second most valuable
if neither cards are trump, A>K>Q>J>10>9>...
ties broken by suit: D>C>H>S
*/
bool Card_less(const Card &a, const Card &b, Suit trump) {
  //if a is trump and b isn't, a must be greater; false
  if(a.is_trump(trump) && !b.is_trump(trump)) {
    return false;
  }
  //if b is trump and a isn't, b must be larger; true
  if(!a.is_trump(trump) && b.is_trump(trump)) {
    return true;
  }
  //both are trump 
  if(a.is_trump(trump) && b.is_trump(trump)) {
    //if a is right bower, a must be greater than b; false
    if(a.is_right_bower(trump)) {
      return false;
    }
    //if b is right bower, b must be greater than a; true
    if(b.is_right_bower(trump)) {
      return true;
    }
    //if neither are right bower, go by left bower
    if(a.is_left_bower(trump)) {
      return false;
    }
    if(b.is_left_bower(trump)) {
      return true;
    }
  }
  return a < b;
}

/*
order as usual, except when a led suit is present
trump suits > led suits > non_trump & non-led suits
*/
bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump) {
  //if neither a and b suit match led card suit, order as usual
  Suit led = led_card.get_suit(trump);
  if(a.get_suit(trump) != led && b.get_suit(trump) != led) {
    return Card_less(a, b, trump);
  }
  //if a is led and b isn't, a greater than b unless b is trump
  if(a.get_suit(trump) == led && b.get_suit(trump) != led) {
    if(b.is_trump(trump)) {
      return true;
    }
    return false;
  }
  //if b is led and a isn't, b greater than a unless a is trump
  if(b.get_suit(trump) == led && a.get_suit(trump) != led) {
    if(a.is_trump(trump)) {
      return false;
    }
    return true;
  }
  //if both are led, compare as usual
  if(a.get_suit(trump) == led && b.get_suit(trump) == led) {
    return Card_less(a, b, trump);
  }
  return a < b;
}