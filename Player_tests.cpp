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
}

TEST_MAIN()
