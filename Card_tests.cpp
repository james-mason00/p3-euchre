#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <cassert>
#include <sstream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_card_def_ctor) {
    Card c;
    ASSERT_EQUAL(c.get_rank(), TWO);
    ASSERT_EQUAL(c.get_suit(), SPADES);
}

TEST(test_get_rank) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(c.get_rank(), ACE);
}

TEST(test_get_suit) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(c.get_suit(), HEARTS);
    
    //check left bower is considered trump suit
    Card c1(JACK, DIAMONDS);
    //if trump is hearts, left bower is hearts as well
    ASSERT_EQUAL(c1.get_suit(HEARTS), HEARTS);
}

TEST(test_is_face_or_ace) {
    Card ace(ACE, DIAMONDS);
    Card king(KING, DIAMONDS);
    Card queen(QUEEN, DIAMONDS);
    Card jack(JACK, DIAMONDS);
    Card not_face_or_ace(FIVE, DIAMONDS);
    assert(ace.is_face_or_ace());
    assert(king.is_face_or_ace());
    assert(queen.is_face_or_ace());
    assert(jack.is_face_or_ace());
    assert(!not_face_or_ace.is_face_or_ace());
}

TEST(test_is_right_bower) {
    Card rBower(JACK, HEARTS);
    Card c(JACK, DIAMONDS);
    assert(rBower.is_right_bower(HEARTS));
    assert(!c.is_right_bower(HEARTS));
    assert(c.is_right_bower(DIAMONDS));
}

TEST(test_is_left_bower) {
    Card lBower(JACK, SPADES);
    Card c(JACK, DIAMONDS);
    assert(lBower.is_left_bower(CLUBS));
    assert(!lBower.is_left_bower(SPADES));
    assert(!c.is_left_bower(SPADES));
}

TEST(test_is_trump) {
    Card c(KING, HEARTS);
    Suit trump = HEARTS;
    Card c1(SEVEN, SPADES);
    assert(c.is_trump(trump));
    assert(!c1.is_trump(trump));
}

TEST(test_output_operator) {
    ostringstream correct;
    correct << "Two of Spades";
    
    Card c;
    ostringstream actual;
    actual << c;
    ASSERT_EQUAL(actual.str(), correct.str());
}

TEST(test_input_operator) {
    istringstream in("Queen of Hearts");

    Card c;
    in >> c;

    assert(c.get_rank() == QUEEN);
    assert(c.get_suit() == HEARTS);
}

TEST(test_less_no_trump_no_led) {
    Card a(NINE, HEARTS);
    Card b(TEN, HEARTS);
    Card a_tie(ACE, SPADES);
    Card b_tie(ACE, DIAMONDS);

    assert(a < b);
    assert(a_tie < b_tie);
}

TEST(test_less_equal_no_trump_no_led) {
    Card a(NINE, HEARTS);
    Card b(TEN, HEARTS);
    Card a_tie(ACE, SPADES);
    Card b_tie(ACE, DIAMONDS);

    assert(a <= b);
    assert(a_tie <= b_tie);
    assert(a <= a);
}

TEST(test_greater_no_trump_no_led) {
    Card a(TEN, HEARTS);
    Card b(NINE, HEARTS);
    Card a_tie(ACE, DIAMONDS);
    Card b_tie(ACE, SPADES);

    assert(a > b);
    assert(a_tie > b_tie);
}

TEST(test_greater_equal_no_trump_no_led) {
    Card a(TEN, HEARTS);
    Card b(NINE, HEARTS);
    Card a_tie(ACE, DIAMONDS);
    Card b_tie(ACE, SPADES);

    assert(a >= b);
    assert(a_tie >= b_tie);
    assert(a_tie >= a_tie);
}

TEST(test_equal_no_trump_no_led) {
    Card a(ACE, HEARTS);
    Card b(ACE, HEARTS);

    assert(a == b);
}

TEST(test_not_equal_no_trump_no_led) {
    Card a(ACE, DIAMONDS);
    Card b(ACE, HEARTS);

    assert(a != b);
}

TEST(test_suit_next) {
    Suit h = HEARTS;
    Suit c = CLUBS;
    Suit s = SPADES;
    Suit d = DIAMONDS;

    //reds
    assert(Suit_next(h) == d);
    assert(Suit_next(d) == h);

    //black
    assert(Suit_next(c) == s);
    assert(Suit_next(s) == c);
}

TEST(test_card_less_normal) {
    Card a(NINE, SPADES);
    Card b(ACE, SPADES);

    assert(Card_less(a, b, HEARTS));
    assert(!Card_less(b, a, HEARTS));
}

TEST(test_card_less_trump) {
    Card a(NINE, HEARTS);
    Card b(ACE, SPADES);

    assert(!Card_less(a, b, HEARTS));
    assert(Card_less(b, a, HEARTS));
}

TEST(test_card_less_right_bower) {
    Card a(JACK, HEARTS);
    Card b(ACE, HEARTS);

    assert(!Card_less(a, b, HEARTS));
    assert(Card_less(b, a, HEARTS));
}

TEST(test_card_less_left_bower) {
    Card a(JACK, DIAMONDS);
    Card b(ACE, HEARTS);

    assert(!Card_less(a, b, HEARTS));
    assert(Card_less(b, a, HEARTS));
}

TEST(test_card_less_bowers) {
    Card a(JACK, HEARTS);
    Card b(JACK, DIAMONDS);

    assert(!Card_less(a, b, HEARTS));
    assert(Card_less(b, a, HEARTS));
}

TEST(test_card_less_trump_order) {
    Card a(NINE, HEARTS);
    Card b(TEN, HEARTS);

    assert(Card_less(a, b, HEARTS));

    Card c(QUEEN, HEARTS);
    Card d(KING, HEARTS);

    assert(Card_less(c, d, HEARTS));

    Card e(KING, HEARTS);
    Card f(ACE, HEARTS);

    assert(Card_less(e, f, HEARTS));
}

TEST(test_card_less_led) {
    Card led(NINE, SPADES);
    Card a(ACE, SPADES);
    Card b(ACE, CLUBS);

    assert(!Card_less(a, b, led, HEARTS));
    assert(Card_less(b, a, led, HEARTS));
}

TEST(test_card_less_trump_over_led) {
    Card led(ACE, SPADES);
    Card a(NINE, HEARTS);

    assert(Card_less(led, a, led, HEARTS));
    assert(!Card_less(a, led, led, HEARTS));
}

TEST(test_card_less_left_bower_led) {
    Card led(NINE, DIAMONDS);
    Card a(JACK, DIAMONDS);
    Card b(ACE, SPADES);

    assert(!Card_less(a, b, led, HEARTS));
    assert(Card_less(b, a, led, HEARTS));
}

TEST(test_card_less_neither_led) {
    Card led(NINE, SPADES);
    Card a(TEN, CLUBS);
    Card b(ACE, DIAMONDS);

    assert(Card_less(a, b, led, HEARTS)
            == Card_less(a, b, HEARTS));
}

TEST_MAIN()
