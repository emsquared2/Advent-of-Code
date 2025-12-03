#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// Compute maximum joltage (choose exactly 12 digits)
long long joltage(const std::string &bank)
{
    const int K = 12;
    int n = bank.size();
    int toRemove = n - K;

    std::string st;
    st.reserve(n);

    for (char c : bank)
    {
        while (!st.empty() && st.back() < c && toRemove > 0)
        {
            st.pop_back();
            --toRemove;
        }
        st.push_back(c);
    }

    // remove leftover digits from the end
    while (toRemove > 0)
    {
        st.pop_back();
        --toRemove;
    }

    // keep exactly first 12 digits
    st.resize(K);

    // convert to long long
    long long value = 0;
    for (char c : st)
    {
        value = value * 10 + (c - '0');
    }
    return value;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " file_path\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open file\n";
        return 1;
    }

    std::string bank;
    long long total = 0;

    while (std::getline(file, bank))
    {
        total += joltage(bank);
    }

    std::cout << "The total output joltage is " << total << "\n";
    return 0;
}
