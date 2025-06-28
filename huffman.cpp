// Huffman File Compressor and Decompressor
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <bitset>
using namespace std;

// Node structure for Huffman Tree
struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, int f) : character(c), frequency(f), left(nullptr), right(nullptr) {}
};

// Comparator for priority queue (min-heap based on frequency)
struct CompareFrequency {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

// Recursively build Huffman codes
void generateCodes(HuffmanNode* root, string currentCode, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCodes[root->character] = currentCode;
    }

    generateCodes(root->left, currentCode + "0", huffmanCodes);
    generateCodes(root->right, currentCode + "1", huffmanCodes);
}

// Compress the input file using Huffman Coding
void compressFile(const string& inputFile, const string& compressedFile, const string& codeFile) {
    ifstream input(inputFile, ios::binary);
    if (!input.is_open()) {
        cerr << "Failed to open input file.\n";
        return;
    }

    unordered_map<char, int> frequencyMap;
    char ch;

    // Count character frequencies
    while (input.get(ch)) {
        frequencyMap[ch]++;
    }

    // Rewind input stream
    input.clear();
    input.seekg(0);

    // Build priority queue (min-heap)
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareFrequency> minHeap;
    for (const auto& [character, freq] : frequencyMap) {
        minHeap.push(new HuffmanNode(character, freq));
    }

    // Build Huffman Tree
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();
        HuffmanNode* merged = new HuffmanNode('\0', left->frequency + right->frequency);
        merged->left = left;
        merged->right = right;
        minHeap.push(merged);
    }

    HuffmanNode* root = minHeap.top();
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Encode input file
    string encodedString;
    while (input.get(ch)) {
        encodedString += huffmanCodes[ch];
    }
    input.close();

    // Add padding bits if necessary
    int paddingBits = 8 - (encodedString.length() % 8);
    if (paddingBits != 8) {
        encodedString.append(paddingBits, '0');
    } else {
        paddingBits = 0;
    }

    // Write compressed binary file
    ofstream output(compressedFile, ios::binary);
    output.put(static_cast<char>(paddingBits)); // Store padding information

    for (size_t i = 0; i < encodedString.size(); i += 8) {
        bitset<8> byte(encodedString.substr(i, 8));
        output.put(static_cast<char>(byte.to_ulong()));
    }
    output.close();

    // Write code map to file
    ofstream codeOut(codeFile);
    for (const auto& [character, code] : huffmanCodes) {
        if (character == '\n') codeOut << "\\n " << code << '\n';
        else if (character == ' ') codeOut << "space " << code << '\n';
        else codeOut << character << " " << code << '\n';
    }
    codeOut.close();

    cout << "Compression complete.\n";
}

// Decompress the Huffman-encoded file
void decompressFile(const string& compressedFile, const string& codeFile, const string& outputFile) {
    unordered_map<string, char> codeToChar;

    // Load Huffman codes from file
    ifstream codeIn(codeFile);
    string line;
    while (getline(codeIn, line)) {
        size_t splitIndex = line.find(' ');
        string symbol = line.substr(0, splitIndex);
        string code = line.substr(splitIndex + 1);
        char character = (symbol == "\\n") ? '\n' : (symbol == "space") ? ' ' : symbol[0];
        codeToChar[code] = character;
    }
    codeIn.close();

    // Read compressed binary data
    ifstream input(compressedFile, ios::binary);
    int paddingBits = input.get();
    string bitStream;

    char byte;
    while (input.get(byte)) {
        bitset<8> bits(static_cast<unsigned char>(byte));
        bitStream += bits.to_string();
    }
    input.close();

    // Remove padding bits
    bitStream = bitStream.substr(0, bitStream.size() - paddingBits);

    // Decode bitstream using the code map
    ofstream output(outputFile);
    string currentCode;
    for (char bit : bitStream) {
        currentCode += bit;
        if (codeToChar.count(currentCode)) {
            output.put(codeToChar[currentCode]);
            currentCode.clear();
        }
    }
    output.close();

    cout << "Decompression complete.\n";
}

int main() {
    string inputFilename = "input.txt";
    string compressedFilename = "compressed.huff";
    string codeMapFilename = "codes.txt";
    string outputFilename = "decompressed.txt";

    compressFile(inputFilename, compressedFilename, codeMapFilename);
    decompressFile(compressedFilename, codeMapFilename, outputFilename);

    return 0;
}
