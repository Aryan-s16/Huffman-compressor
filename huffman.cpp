// Huffman File Compressor + Decompressor in C++
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <bitset>
using namespace std;

// Node for Huffman Tree
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode *left, *right;
    HuffmanNode(char c, int f) : ch(c), freq(f), left(NULL), right(NULL) {}
};

// Custom compare for priority queue
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

// Recursive function to build codes
void buildCodes(HuffmanNode* root, string code, unordered_map<char, string>& codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->ch] = code;
    }
    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

// Compress file
void compress(const string& inputFile, const string& outputFile, const string& codeFile) {
    ifstream in(inputFile, ios::binary);
    unordered_map<char, int> freq;
    char ch;
    while (in.get(ch)) freq[ch]++;
    in.clear(); in.seekg(0);

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (auto& p : freq) pq.push(new HuffmanNode(p.first, p.second));

    while (pq.size() > 1) {
        HuffmanNode *left = pq.top(); pq.pop();
        HuffmanNode *right = pq.top(); pq.pop();
        HuffmanNode *merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    HuffmanNode* root = pq.top();
    unordered_map<char, string> codes;
    buildCodes(root, "", codes);

    ofstream out(outputFile, ios::binary);
    string encoded;
    while (in.get(ch)) encoded += codes[ch];

    int extraBits = 8 - (encoded.size() % 8);
    if (extraBits == 8) extraBits = 0;
    for (int i = 0; i < extraBits; ++i) encoded += '0';

    out.put(extraBits);
    for (size_t i = 0; i < encoded.size(); i += 8) {
        bitset<8> b(encoded.substr(i, 8));
        out.put((char)b.to_ulong());
    }

    ofstream codeOut(codeFile);
    for (auto& p : codes) {
        if (p.first == '\n') codeOut << "\\n " << p.second << '\n';
        else if (p.first == ' ') codeOut << "space " << p.second << '\n';
        else codeOut << p.first << " " << p.second << '\n';
    }

    in.close(); out.close(); codeOut.close();
    cout << "Compression complete!\n";
}

// Decompress file
void decompress(const string& inputFile, const string& codeFile, const string& outputFile) {
    unordered_map<string, char> reverseCodes;
    ifstream codeIn(codeFile);
    string line;
    while (getline(codeIn, line)) {
        size_t space = line.find(' ');
        string key = line.substr(0, space);
        string val = line.substr(space + 1);
        char realKey = (key == "\\n") ? '\n' : (key == "space") ? ' ' : key[0];
        reverseCodes[val] = realKey;
    }
    codeIn.close();

    ifstream in(inputFile, ios::binary);
    int padding = in.get();
    string bits = "";
    char ch;
    while (in.get(ch)) {
        bitset<8> b((unsigned char)ch);
        bits += b.to_string();
    }
    in.close();

    bits = bits.substr(0, bits.size() - padding);
    string current = "";
    ofstream out(outputFile);
    for (char bit : bits) {
        current += bit;
        if (reverseCodes.count(current)) {
            out.put(reverseCodes[current]);
            current = "";
        }
    }
    out.close();
    cout << "Decompression complete!\n";
}

int main() {
    string input = "input.txt";
    string compressed = "output.huff";
    string codes = "codes.txt";
    string decompressed = "decoded.txt";

    compress(input, compressed, codes);
    decompress(compressed, codes, decompressed);
    return 0;
}
