#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

/*
 Represents one hand from the input:
 - cards: the 5 cards as given (order matters!)
 - bid: the bid associated with the hand
 - type: the classified hand strength (enum HandType)
*/
struct Hand
{
    std::string cards; // e.g. "32T3K"
    int bid;
    int type; // strength of hand (HIGH_CARD .. FIVE_KIND)
};

enum HandType
{
    HIGH_CARD = 1,
    ONE_PAIR,
    TWO_PAIR,
    THREE_KIND,
    FULL_HOUSE,
    FOUR_KIND,
    FIVE_KIND
};

int card_strength(char c)
{
    static const std::string order = "23456789TJQKA";
    return order.find(c);
}

int get_type(const std::string &hand)
{
    // Count frequency of each card label
    std::unordered_map<char, int> freq;
    for (char c : hand)
        freq[c]++;

    // Collect the frequencies into a vector
    std::vector<int> counts;
    for (auto &[_, v] : freq)
        counts.push_back(v);

    // Sort descending (e.g. [3,2] for full house)
    std::sort(counts.begin(), counts.end(), std::greater<>());

    // Match against known hand shapes
    if (counts == std::vector<int>{5})
        return FIVE_KIND;
    if (counts == std::vector<int>{4, 1})
        return FOUR_KIND;
    if (counts == std::vector<int>{3, 2})
        return FULL_HOUSE;
    if (counts == std::vector<int>{3, 1, 1})
        return THREE_KIND;
    if (counts == std::vector<int>{2, 2, 1})
        return TWO_PAIR;
    if (counts == std::vector<int>{2, 1, 1, 1})
        return ONE_PAIR;
    return HIGH_CARD;
}

bool weaker(const Hand &a, const Hand &b)
{
    // Primary comparison: hand type
    if (a.type != b.type)
        return a.type < b.type;

    // Secondary comparison: card-by-card (left to right)
    for (int i = 0; i < 5; ++i)
    {
        int sa = card_strength(a.cards[i]);
        int sb = card_strength(b.cards[i]);
        if (sa != sb)
            return sa < sb;
    }

    // Hands are identical in strength
    return false;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::vector<Hand> hands;
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        Hand h;
        ss >> h.cards >> h.bid;
        h.type = get_type(h.cards);
        hands.push_back(h);
    }

    std::sort(hands.begin(), hands.end(), weaker);

    long long total = 0;
    for (int i = 0; i < hands.size(); ++i)
    {
        total += 1LL * hands[i].bid * (i + 1);
    }

    std::cout << total << "\n";
    return 0;
}