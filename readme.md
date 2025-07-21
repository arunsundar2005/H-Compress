
# Huffman Compression CLI Tool

## 📦 Project Overview

This project implements a **Huffman Encoding Compression and Decompression System** as a **Command Line Interface (CLI) tool** for Windows. It takes an input file, compresses it using Huffman coding, and stores the compressed output in a custom format (`.hc`). It can also decompress the `.hc` file back to its original form.

The executable (`.exe`) version of this tool is located in the **CLI Tool folder**.

---
## 🗂 Repository Structure

```
CLI Tool/
├── tool.exe
├── message.txt
├── compressed.hc
├── output.txt
└── README.md
```

## ⚙️ Features

- Lossless text compression using Huffman encoding.
- Stores compressed data and frequency table in a custom file format.
- Decompression uses the frequency table to rebuild the Huffman tree.
- Simple CLI interface.
- Clean modular C code.

---

## 🧠 How It Works

### 1. Frequency Calculation

```c
int *freq_calc(char *text) {
    for (int i = 0; text[i]; i++) {
        freq[(unsigned char)text[i]]++;
    }
    return freq;
}
```

- A frequency table (`freq[256]`) is created using the input text.
- Each character's ASCII value is used to index and count occurrences.

---

### 2. Huffman Tree Construction

- A min-heap is used to construct the Huffman tree.
- Each node represents a character and its frequency.
- Nodes are combined until one root remains.

---

### 3. Generating Huffman Codes

- Traverse the tree recursively.
- Left = 0, Right = 1.
- Store codes in a lookup table.

---

### 4. Compression

```c
compress(argv[2], avg[1]);
```
- Where the `argv[1]` is the input file and `argv[2]` is the output file.
- **Note**: The ouput file, here (i.e., the `argv[2]`) contains only the name of the compressed file, **NOT** the extension.
- The user gives an input file.
- Huffman codes replace characters in the file.
- Frequency table is written at the start of the `.hc` file.
- The final bitstream is written after the table.

---

### 5. Decompression

```c
decompress_from_file(argv[2], argv[3]);
```
- Where the `argv[2]` is the input file (i.e., the compressed file that has to be extracted) and `argv[3]` is the output file.
- The `.hc` file is read.
- Frequency table is reconstructed.
- Huffman tree is built again.
- Bitstream is decoded to original text.

---

## 🧪 CLI Usage

The CLI tool supports two commands:

```bash
> tool.exe -c <input_file.txt> <output_file_name>
> tool.exe -d <input_file.hc> <output_file.txt>
```

### Example:

```bash
> tool.exe -c message.txt compressed
> tool.exe -d compressed.hc output.txt
```

### Output Extensions:

- For compression, `.hc` is added automatically.
- For decompression, a normal `.txt` file is restored.

---



## 💻 Notes

- The program uses standard C libraries only.
- `memset(freq, 0, sizeof(freq));` is used to initialize arrays.
- `strcat(output_file, ".hc");` is used to append `.hc` extension.

---

## ✅ Future Improvements

- Support binary files.
- GUI wrapper over CLI.
- Add compression ratio reporting.
- Allow output file paths with directories.

---

## 🧑‍💻 Author

Developed by **Arun Sundar**, 2025.

---

## 📌 License

This project is open-source and free to use under the MIT License.
