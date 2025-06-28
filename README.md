# Huffman File Compressor & Decompressor (C++)

This project implements a **Huffman Coding-based file compressor and decompressor** in C++. It compresses a plain text file into a `.huff` binary file and can decompress it back to its original form using a saved code map.

---

## 🚀 Features

- ✅ Compress any text file using **Huffman Encoding**
- ✅ Decompress `.huff` binary files back to original text
- ✅ Saves Huffman code mapping to a `.txt` file
- ✅ Bit-level binary I/O for efficient space utilization
- ✅ Fully written in modern C++ with STL support

---

## 📁 Project Structure

```
.
├── huffman.cpp         # Main C++ source code (compressor + decompressor)
├── input.txt           # Sample input text file
├── output.huff         # Compressed binary output
├── codes.txt           # Huffman code mappings
├── decoded.txt         # Output after decompression
├── README.md           # Project documentation (this file)
└── .gitignore          # Ignores binary and intermediate files
```

---

## ⚙️ How to Compile and Run

### 🛠 Compile:
```bash
g++ huffman.cpp -o huffman
```

### ▶️ Run:
```bash
./huffman
```

This will:
- Compress `input.txt` → `output.huff`
- Save code map to `codes.txt`
- Decompress `output.huff` back → `decoded.txt`

---

## 📌 Example Workflow

1. Add your text to `input.txt`
2. Run the program
3. Check:
   - `output.huff` → compressed binary
   - `codes.txt` → saved Huffman codes
   - `decoded.txt` → original file restored

---

## 📦 Sample Output

```
Compression complete!
Decompression complete!
```

---

## 🔍 How It Works

- **Compression**:
  - Calculate character frequencies
  - Build a Huffman tree using a min-heap
  - Generate binary codes (shorter for frequent characters)
  - Write compressed binary to `output.huff`
  - Save codes in `codes.txt`

- **Decompression**:
  - Read `codes.txt` to reconstruct codes
  - Read binary `output.huff`, convert back to bitstream
  - Decode using the map and write to `decoded.txt`

---

## 🙌 Author

**Aryan**  
C++ Developer | DSA Enthusiast | SDE Aspirant

---

## 📜 License

This project is open-source and free to use under the [MIT License](https://opensource.org/licenses/MIT).

---

## ⭐️ Give a Star!

If you like this project, consider giving it a ⭐️ on GitHub — it helps others find it too!

