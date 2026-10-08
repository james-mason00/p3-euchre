#include "Player.hpp"
#include "unit_test_framework.hpp"
#include <cassert>
#include <iostream>

using namespace std;

TEST(test_simple_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_player_insertion) {
  // Create a Human player
  Player * human = Player_factory("NotRobot", "Human");

    // Print the player using the stream insertion operator
    ostringstream oss1;
    oss1 << * human;

    // Verify that the output is the player's name
    ASSERT_EQUAL(oss1.str(), "NotRobot");

    // Create a Simple player
    Player * alice = Player_factory("Alice", "Simple");

    // Print the player using the stream insertion operator
    ostringstream oss2;
    oss2 << *alice;
    ASSERT_EQUAL(oss2.str(), "Alice");

    // Clean up players that were created using Player_factory()
    delete human;
    delete alice;
}

TEST(test_human_player_get_name) {
    Player * james =  Player_factory("James", "Human");
    ASSERT_EQUAL("James", james->get_name());

    delete james;
}

TEST(test_simple_make_trump_round_1) {
    Player * james = Player_factory("James", "Simple");
    james->add_card(Card(JACK, HEARTS));
    james->add_card(Card(QUEEN, HEARTS));
    james->add_card(Card(NINE, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, DIAMONDS));

    Suit order_up_suit = SPADES;
    bool order_up = james->make_trump(Card(ACE, HEARTS), true, 1, order_up_suit);

    assert(order_up);
    ASSERT_EQUAL(HEARTS, order_up_suit);

    delete james;
}

TEST(test_simple_make_trump_round_2) {
    Player * james = Player_factory("James", "Simple");
    james->add_card((Card(NINE, DIAMONDS)));
    james->add_card((Card(ACE, DIAMONDS)));
    james->add_card((Card(KING, SPADES)));
    james->add_card((Card(TEN, CLUBS)));
    james->add_card((Card(NINE, HEARTS)));

    Suit order_up_suit = SPADES;
    assert(james->make_trump(Card(TEN, HEARTS), true, 2, order_up_suit));
    ASSERT_EQUAL(DIAMONDS, order_up_suit);

    delete james;
}

TEST(test_simple_make_trump) {
    Player * james = Player_factory("james", "Simple");
    Player * jonno = Player_factory("jonno", "Simple");

    Card up_card =  Card(TEN, DIAMONDS);
    Suit order_up_suit = CLUBS;

    //has right and left bower, should order up
    james->add_card(Card(JACK, DIAMONDS));
    james->add_card(Card(JACK, HEARTS));
    james->add_card(Card(FOUR, CLUBS));
    james->add_card(Card(SEVEN, HEARTS));
    james->add_card(Card(FOUR, SPADES));

    assert(james->make_trump(up_card, false, 1, order_up_suit));
    assert(order_up_suit == DIAMONDS);
    assert(james->make_trump(up_card, true, 2, order_up_suit));
    assert(order_up_suit == HEARTS);

    Card up_card1 = Card(KING, HEARTS);
    Suit order_up_suit1 = CLUBS;

    //every card in jonnos hands is led next suit, but only one is face/ace (rbower)
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(FOUR, DIAMONDS));
    jonno->add_card(Card(THREE, DIAMONDS));
    jonno->add_card(Card(NINE, DIAMONDS));
    jonno->add_card(Card(SEVEN, DIAMONDS));

    ASSERT_FALSE(jonno->make_trump(up_card1, false, 1, order_up_suit1));
    assert(order_up_suit1 == CLUBS);
    assert(jonno->make_trump(up_card1, false, 2, order_up_suit1));
    assert(order_up_suit1 == DIAMONDS);

    delete james;
    delete jonno;
}

TEST(test_simple_make_trump_lbower) {
    Player * james =  Player_factory("james", "Simple");

    Card upCard = Card(SEVEN, HEARTS);
    Suit order_up_suit = CLUBS;

    //lbower considered part of trump suit 
    james->add_card(Card(JACK, DIAMONDS));
    james->add_card(Card(ACE, HEARTS));
    james->add_card(Card(FOUR, CLUBS));
    james->add_card(Card(THREE, SPADES));
    james->add_card(Card(FIVE, SPADES));

    assert(james->make_trump(upCard, true, 1, order_up_suit));
    assert(order_up_suit == HEARTS);

    delete james;
}

TEST(test_simple_make_trump_1face) {
    Player * james =  Player_factory("james", "Simple");

    Card upcard(TEN, DIAMONDS);
    Suit order_up = CLUBS;

    //only 1 diamond face or ace
    james->add_card(Card(NINE, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(FOUR, DIAMONDS));
    james->add_card(Card(TEN, HEARTS));
    james->add_card(Card(QUEEN, DIAMONDS));

    //for round 1, don't order up
    ASSERT_FALSE(james->make_trump(upcard, false, 1, order_up));
    assert(order_up == CLUBS);

    delete james;
}

TEST(test_simple_make_trump_round_22) {
    Player * james = Player_factory("james", "Simple");

    Card upcard(THREE, HEARTS);

    //only need 1 face or ace of next suit
    james->add_card(Card(ACE, DIAMONDS));
    james->add_card(Card(NINE, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, HEARTS));
    james->add_card(Card(TEN, DIAMONDS));

    Suit order_up_suit = SPADES;

    assert(james->make_trump(upcard, false, 2, order_up_suit));
    assert(order_up_suit == DIAMONDS);

    delete james;
}

TEST(test_simple_make_trump_dealer_round_2) {
    Player * james = Player_factory("james", "Simple");

    Card upcard(THREE, HEARTS);
    Suit order_up_suit = SPADES;

    //screw the dealer
    james->add_card(Card(TWO, SPADES));
    james->add_card(Card(THREE, SPADES));
    james->add_card(Card(FOUR, SPADES));
    james->add_card(Card(FIVE, SPADES));
    james->add_card(Card(SIX, SPADES));

    assert(james->make_trump(upcard, true, 2, order_up_suit));
    //suit is next suit
    assert(order_up_suit == DIAMONDS);

    delete james;
}

TEST(test_simple_add_and_discard) {
    Player *james = Player_factory("James", "Simple");

    james->add_card(Card(NINE, DIAMONDS));
    james->add_card(Card(ACE, DIAMONDS));
    james->add_card(Card(KING, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, HEARTS));

    Card upcard(ACE, HEARTS);
    james->add_and_discard(upcard);
    // 9 of diamonds should've been discarded
    ASSERT_NOT_EQUAL(Card(NINE, DIAMONDS), james->lead_card(CLUBS));

    delete james;
}

TEST(test_simple_add_and_discard_upcard) {
    Player *jonno = Player_factory("i_miss_you", "Simple");

    jonno->add_card(Card(NINE, DIAMONDS));
    jonno->add_card(Card(TEN, DIAMONDS));
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(QUEEN, DIAMONDS));
    jonno->add_card(Card(KING, DIAMONDS));

    Card upcard(EIGHT, DIAMONDS);
    jonno->add_and_discard(upcard);

    //upcard (eight of diamonds) should've been discarded
    ASSERT_NOT_EQUAL(upcard, jonno->lead_card(CLUBS));
    ASSERT_NOT_EQUAL(upcard, jonno->lead_card(CLUBS));
    ASSERT_NOT_EQUAL(upcard, jonno->lead_card(CLUBS));
    ASSERT_NOT_EQUAL(upcard, jonno->lead_card(CLUBS));
    ASSERT_NOT_EQUAL(upcard, jonno->lead_card(CLUBS));

    delete jonno;
}

TEST(test_simple_lead_card) {
    Player * james = Player_factory("james", "Simple");
    Player * jake = Player_factory("jake", "Simple");
    Player * jonno = Player_factory("i_love_you", "Simple");

    //james has normal hand, will lead highest non trump
    james->add_card(Card(NINE, DIAMONDS));
    james->add_card(Card(ACE, DIAMONDS));
    james->add_card(Card(KING, SPADES));
    james->add_card(Card(TEN, CLUBS));
    james->add_card(Card(NINE, HEARTS));

    //if trump is diamonds, should lead king of spades
    ASSERT_EQUAL(james->lead_card(DIAMONDS), Card(KING, SPADES));

    //jake only has trumps
    jake->add_card(Card(ACE, DIAMONDS));
    jake->add_card(Card(KING, DIAMONDS));
    jake->add_card(Card(QUEEN, DIAMONDS));
    jake->add_card(Card(NINE, DIAMONDS));
    jake->add_card(Card(TEN, DIAMONDS));

    //jake should lead with highest card in his hand
    ASSERT_EQUAL(jake->lead_card(DIAMONDS), Card(ACE, DIAMONDS));

    //jonno has right bower and left bower
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(JACK, HEARTS));
    jonno->add_card(Card(ACE, HEARTS));
    jonno->add_card(Card(TWO, SPADES));
    jonno->add_card(Card(THREE, SPADES));

    //if trump is diamonds, jonno should lead with ace of hearts
    ASSERT_EQUAL(jonno->lead_card(DIAMONDS), Card(ACE, HEARTS));

    delete james;
    delete jonno;
    delete jake;
}

TEST(test_simple_lead_lbower) {
    Player * jonno = Player_factory("jonno", "Simple");

    //trump is hearts
    jonno->add_card(Card(ACE, HEARTS));
    jonno->add_card(Card(KING, HEARTS));
    jonno->add_card(Card(QUEEN, SPADES));   //lowest non trump
    jonno->add_card(Card(TEN, HEARTS));
    jonno->add_card(Card(JACK, DIAMONDS));  //lbower, part of trump!
    
    ASSERT_EQUAL(jonno->lead_card(HEARTS), Card(QUEEN, SPADES));

    delete jonno;

}

TEST(test_simple_play_card) {
    Player * james = Player_factory("james", "Simple");
    Player * jonno = Player_factory("goat", "Simple");

    //james can follow led suit (hearts)
    james->add_card(Card(ACE, HEARTS));
    james->add_card(Card(NINE, HEARTS));
    james->add_card(Card(TWO, SPADES));
    james->add_card(Card(FOUR, CLUBS));
    james->add_card(Card(ACE, DIAMONDS));

    //james should play ace of hearts
    ASSERT_EQUAL(james->play_card(Card(THREE, HEARTS), SPADES), Card(ACE, HEARTS));

    //jonno can't follow suit (hearts)
    jonno->add_card(Card(ACE, DIAMONDS));
    jonno->add_card(Card(KING, DIAMONDS));
    jonno->add_card(Card(QUEEN, DIAMONDS));
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(NINE, DIAMONDS));

    //jonno should play nine of diamonds
    ASSERT_EQUAL(jonno->play_card(Card(THREE, HEARTS), SPADES), Card(NINE, DIAMONDS));

    delete james;
    delete jonno;
}

TEST(test_simple_play_lbower) {
    Player * james = Player_factory("james", "Simple");
    Player * jonno = Player_factory("jonno", "Simple");

    Card ledCard(TEN, DIAMONDS);

    //james can follow suit with lbower and one other, play lbower bc it's higher
    james->add_card(Card(JACK, HEARTS));
    james->add_card(Card(TEN, DIAMONDS));
    james->add_card(Card(TWO, SPADES));
    james->add_card(Card(THREE, SPADES));
    james->add_card(Card(FOUR, SPADES));

    //james should play jack of hearts
    ASSERT_EQUAL(james->play_card(ledCard, DIAMONDS), Card(JACK, HEARTS));

    //jonno can't follow suit bc lbower is trump suit (HEARTS), not led suit
    jonno->add_card(Card(JACK, DIAMONDS));
    jonno->add_card(Card(TWO, SPADES));
    jonno->add_card(Card(THREE, SPADES));
    jonno->add_card(Card(FOUR, SPADES));
    jonno->add_card(Card(FIVE, SPADES));

    //jonno should play two of spades
    ASSERT_EQUAL(jonno->play_card(ledCard, HEARTS), Card(TWO, SPADES));

    delete james;
    delete jonno;
}

TEST_MAIN()
