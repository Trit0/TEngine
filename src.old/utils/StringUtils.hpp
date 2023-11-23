#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP
namespace GEngine {
class StringUtils {
   public:
    static string* Split(string textToSplit, char delimiter = 0) {
        vector<string> value = vector<string>();
        int s = 0;
        for (int i = 0; i < textToSplit.length(); i++) {
            if (textToSplit[i] == delimiter || delimiter == 0) {
                value.push_back(textToSplit.substr(s, i));
                textToSplit = textToSplit.substr(i + 1, textToSplit.length() - 1);
                i = -1;
            }
        }
        value.push_back(textToSplit);
        string* v = new string[value.size()];
        int i = 0;
        for (string s : value) {
            v[i] = value[i];
            i++;
        }
        return v;
    }
};

}  // namespace GEngine

#endif
