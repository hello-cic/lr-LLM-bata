#ifndef u8tou32_hpp
#define u8tou32_hpp

#include <string>
using std::string;
using std::u32string;

#include <algorithm>

#include <vector>

class mc {
public:
    static u32string utf8_to_u32(const string& s) {
        std::u32string out;
        for (size_t i = 0; i < s.size(); ) {
            unsigned char c = s[i];
            char32_t cp = 0;
            int len = 0;

            if ((c & 0x80) == 0) { cp = c; len = 1; }
            else if ((c & 0xE0) == 0xC0) { cp = c & 0x1F; len = 2; }
            else if ((c & 0xF0) == 0xE0) { cp = c & 0x0F; len = 3; }
            else if ((c & 0xF8) == 0xF0) { cp = c & 0x07; len = 4; }
            else { out += U'?'; ++i; continue; }

            if (i + len > s.size()) { out += U'?'; break; }

            for (int j = 1; j < len; ++j) {
                unsigned char n = s[i + j];
                if ((n & 0xC0) != 0x80) { cp = U'?'; break; }
                cp = (cp << 6) | (n & 0x3F);
            }
            out += cp;
            i += len;
        }
        return out;
    }

    static std::vector<int> bin(int input) {
        std::vector<int> result;
        if (input == 0) {
            result.push_back(0);
            return result;
        }
        while (input > 0) {
            result.push_back(input % 2);
            input /= 2;
        }
        std::reverse(result.begin(), result.end());
        return result;
    }
};

#endif
