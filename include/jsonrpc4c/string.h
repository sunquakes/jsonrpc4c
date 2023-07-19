#ifndef JSONRPC4C_STRING_H
#define JSONRPC4C_STRING_H

namespace jsonrpc4c {
    std::vector<std::string> split(const std::string& input, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(input);
        std::string token;

        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }
};

#endif //JSONRPC4C_STRING_H
