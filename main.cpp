#define STL_ALIASES
#define STL_HELPERS
#include "Types.h"

#include <array>
#include <iostream>
#include <random>

enum class Suit {
    Clubs,
    Diamonds,
    Hearts,
    Spades,
};

const Map<Suit, str> SuitNames = {
  {Suit::Clubs, "Clubs"},
  {Suit::Diamonds, "Diamonds"},
  {Suit::Hearts, "Hearts"},
  {Suit::Spades, "Spades"},
};

const Map<i32, str> RankNames = {
  {1, "Ace"},
  {2, "2"},
  {3, "3"},
  {4, "4"},
  {5, "5"},
  {6, "6"},
  {7, "7"},
  {8, "8"},
  {9, "9"},
  {10, "10"},
  {11, "Jack"},
  {12, "Queen"},
  {13, "King"},
};

enum class Rank {
    Two = 2,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

enum class HandRank {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush,
    RoyalFlush
};

struct Card {
    Suit suit;
    Rank rank;

    bool operator<(const Card& other) const {
        if (suit != other.suit)
            return suit < other.suit;
        return rank < other.rank;
    }

    [[nodiscard]] str Name() const {
        return RankNames.at((i32)rank) + " of " + SuitNames.at(suit);
    }
};

struct Hand {
    std::array<Card, 2> cards;
};

class Deck {
public:
    std::deque<Card> cards;
    std::array<Card, 5> communityCards;

    Deck() : communityCards({}) {
        for (const Suit suit : {Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades}) {
            for (i32 rank = 1; rank <= 13; ++rank) {
                cards.push_back({suit, (Rank)rank});
            }
        }
    }

    void Shuffle(i32 iterations = 7) {
        for (i32 i = 0; i < iterations; ++i) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::ranges::shuffle(cards, gen);
        }
    }

    void Print() const {
        for (const Card& card : cards) {
            printf("%s\n", card.Name().c_str());
        }
    }

    Hand DealHand() {
        if (cards.size() < 2) {
            throw std::out_of_range("Not enough cards to deal a hand");
        }

        Hand deal = {};

        deal.cards[0] = cards.front();
        cards.pop_front();

        deal.cards[1] = cards.front();
        cards.pop_front();

        return deal;
    }

    void Flop() {
        if (cards.size() < 3) {
            throw std::out_of_range("Not enough cards to deal a hand");
        }

        for (auto i = 0; i < 3; ++i) {
            communityCards[i] = cards.front();
            cards.pop_front();
        }
    }

    void Turn() {
        if (cards.empty()) {
            throw std::out_of_range("Not enough cards to deal a hand");
        }

        communityCards[3] = cards.front();
        cards.pop_front();
    }

    void River() {
        if (cards.empty()) {
            throw std::out_of_range("Not enough cards to deal a hand");
        }

        communityCards[4] = cards.front();
        cards.pop_front();
    }
};

Map<Rank, int> GetRankCount(const Vector<Card>& cards) {
    std::map<Rank, int> rankCount;
    for (const auto& card : cards) {
        rankCount[card.rank]++;
    }
    return rankCount;
}

bool IsFlush(const Vector<Card>& cards) {
    Map<Suit, int> suitCount;
    for (const auto& card : cards) {
        suitCount[card.suit]++;
    }

    return std::ranges::any_of(suitCount,
                               [](std::pair<Suit, int> pair) { return pair.second >= 5; });
}

bool IsStraight(const Vector<Card>& cards) {
    Vector<Rank> ranks;
    for (const auto& card : cards) {
        ranks.push_back(card.rank);
    }

    std::ranges::sort(ranks);

    // Remove duplicates
    const auto last = std::ranges::unique(ranks).begin();
    ranks.erase(last, ranks.end());

    // Check for a straight
    int consecutiveCount = 1;
    for (size_t i = 1; i < ranks.size(); ++i) {
        if (static_cast<int>(ranks[i]) == static_cast<int>(ranks[i - 1]) + 1) {
            consecutiveCount++;
            if (consecutiveCount >= 5) {
                return true;
            }
        } else if (static_cast<int>(ranks[i]) != static_cast<int>(ranks[i - 1])) {
            consecutiveCount = 1;
        }
    }
    return false;
}

HandRank EvaluateHand(const Vector<Card>& cards) {
    auto rankCount      = GetRankCount(cards);
    const bool flush    = IsFlush(cards);
    const bool straight = IsStraight(cards);

    if (flush && straight) {
        // Check for straight flush
        if (rankCount[Rank::Ace] && rankCount[Rank::King] && rankCount[Rank::Queen] &&
            rankCount[Rank::Jack] && rankCount[Rank::Ten]) {
            return HandRank::RoyalFlush;
        }
        return HandRank::StraightFlush;
    }
    if (rankCount.size() == 2) {
        if (rankCount.begin()->second == 4 || rankCount.rbegin()->second == 4) {
            return HandRank::FourOfAKind;
        } else {
            return HandRank::FullHouse;
        }
    }
    if (rankCount.size() == 3) {
        if (rankCount.begin()->second == 3 || rankCount.rbegin()->second == 3) {
            return HandRank::ThreeOfAKind;
        } else {
            return HandRank::TwoPair;
        }
    }
    if (rankCount.size() == 4) {
        return HandRank::OnePair;
    }
    if (flush) {
        return HandRank::Flush;
    }
    if (straight) {
        return HandRank::Straight;
    }
    return HandRank::HighCard;
}

int main() {
    Deck deck;
    deck.Shuffle();

    Hand playerHand = deck.DealHand();

    deck.Flop();
    deck.Turn();
    deck.River();

    // Analyze player's hand
    {
        auto community = deck.communityCards;
        auto hand      = playerHand.cards;
        std::vector<Card> inPlay;

        // Insert elements from both arrays
        inPlay.insert(inPlay.end(), community.begin(), community.end());
        inPlay.insert(inPlay.end(), hand.begin(), hand.end());

        // Sort the combined vector
        std::sort(inPlay.begin(), inPlay.end());

        const auto result = EvaluateHand(inPlay);

        for (auto card : inPlay) {
            std::cout << "| " << card.Name() << " ";
        }
        std::cout << std::endl;

        switch (result) {
            case HandRank::HighCard:
                std::cout << "High Card\n";
                break;
            case HandRank::OnePair:
                std::cout << "One Pair\n";
                break;
            case HandRank::TwoPair:
                std::cout << "Two Pair\n";
                break;
            case HandRank::ThreeOfAKind:
                std::cout << "Three of a Kind\n";
                break;
            case HandRank::Straight:
                std::cout << "Straight\n";
                break;
            case HandRank::Flush:
                std::cout << "Flush\n";
                break;
            case HandRank::FullHouse:
                std::cout << "Full House\n";
                break;
            case HandRank::FourOfAKind:
                std::cout << "Four of a Kind\n";
                break;
            case HandRank::StraightFlush:
                std::cout << "Straight Flush\n";
                break;
            case HandRank::RoyalFlush:
                std::cout << "Royal Flush\n";
                break;
        }
    }

    return 0;
}
