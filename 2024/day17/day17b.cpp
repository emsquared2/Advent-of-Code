#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <iterator>

using namespace std;

vector<long long> run(const vector<long long> &program, const vector<long long> &regs)
{
    const long long a = 4, b = 5, c = 6;
    long long ip = 0;
    vector<long long> combo = {0, 1, 2, 3};
    combo.insert(combo.end(), regs.begin(), regs.end());
    vector<long long> output;

    while (ip < program.size())
    {
        long long opcode = program[ip];
        long long operand = program[ip + 1];
        switch (opcode)
        {
        case 0:
            combo[a] >>= combo[operand];
            break;
        case 1:
            combo[b] ^= operand;
            break;
        case 2:
            combo[b] = combo[operand] % 8;
            break;
        case 3:
            if (combo[a])
            {
                ip = operand - 2;
            }
            break;
        case 4:
            combo[b] ^= combo[c];
            break;
        case 5:
            output.push_back(combo[operand] % 8);
            break;
        case 6:
            combo[b] = combo[a] >> combo[operand];
            break;
        case 7:
            combo[c] = combo[a] >> combo[operand];
            break;
        }
        ip += 2;
    }

    return output;
}

long long expect(const vector<long long> &program, const vector<long long> &out, long long prev_a = 0)
{
    if (out.empty())
    {
        return prev_a;
    }

    for (long long a = 0; a < (1 << 10); ++a)
    {
        if ((a >> 3) == (prev_a & 127) && run(program, {a, 0, 0})[0] == out.back())
        {
            long long ret = expect(program, vector<long long>(out.begin(), out.end() - 1), (prev_a << 3) | (a % 8));
            if (ret != -1)
            {
                return ret;
            }
        }
    }

    return -1;
}

vector<long long> parse_input(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    regex re("\\d+");
    sregex_iterator begin(content.begin(), content.end(), re), end;
    vector<long long> nums;

    for (auto it = begin; it != end; ++it)
    {
        nums.push_back(stoi(it->str()));
    }

    return nums;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    vector<long long> nums = parse_input(argv[1]);

    vector<long long> regs(nums.begin(), nums.begin() + 3);
    vector<long long> program(nums.begin() + 3, nums.end());

    vector<long long> output = run(program, regs);
    for (size_t i = 0; i < output.size(); ++i)
    {
        if (i > 0)
            cout << ",";
        cout << output[i];
    }
    cout << endl;

    cout << expect(program, program) << endl;

    return 0;
}
