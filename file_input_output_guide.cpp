/*
  File: file_input_output_guide.cpp

  An extensive, practical guide to file input/output (I/O) in modern C++.

  Build:
    g++ -std=c++20 -O2 -Wall -Wextra -pedantic file_input_output_guide.cpp -o file_io_guide

  Run:
    ./file_io_guide

  Notes:
  - This file is meant to be read like a tutorial and also compiles/runs.
  - It writes a small set of demo files into ./io_guide_output/.
  - Examples use <fstream> (std::ifstream/std::ofstream/std::fstream) and
    <filesystem> for path handling.
*/

#include <algorithm>
#include <array>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

// ------------------------------------------------------------
// 0) Quick mental model
// ------------------------------------------------------------
/*
  C++ file I/O typically uses iostreams:

    - std::ifstream : input from a file (read)
    - std::ofstream : output to a file (write)
    - std::fstream  : input + output (read/write)

  They are all "streams". Streams maintain:

    - a buffer (for performance)
    - state flags: goodbit, eofbit, failbit, badbit
    - a position (get pointer for reading, put pointer for writing)

  Important operations:
    - open/close, is_open
    - operator>> for formatted extraction (skips whitespace by default)
    - std::getline for line-based extraction
    - read/write for binary/unformatted I/O
    - seekg/seekp and tellg/tellp for random access

  The single best habit:
    Always check that the file opened successfully and handle errors.
*/

// ------------------------------------------------------------
// 1) Paths and portability
// ------------------------------------------------------------
/*
  Prefer std::filesystem::path over raw strings when dealing with file paths.
  It handles platform separators and conversions.

  Examples:
    fs::path p = "data/input.txt";
    p /= "more";  // path concatenation

  To print a path reliably:
    std::cout << p << "\n";   // streams know how to print paths
*/

// ------------------------------------------------------------
// 2) Opening files: modes
// ------------------------------------------------------------
/*
  open modes (std::ios::openmode):

    std::ios::in     : open for reading
    std::ios::out    : open for writing (can create)
    std::ios::app    : append (all writes go to end)
    std::ios::trunc  : truncate existing file when opening for output
    std::ios::binary : binary mode (no newline translation)

  Defaults:
    - ifstream defaults to std::ios::in
    - ofstream defaults to std::ios::out | std::ios::trunc

  Common patterns:
    - Read text:        std::ifstream f(path);
    - Write text:       std::ofstream f(path);           // truncates
    - Append text:      std::ofstream f(path, std::ios::app);
    - Read binary:      std::ifstream f(path, std::ios::binary);
    - Write binary:     std::ofstream f(path, std::ios::binary);
    - Read/write:       std::fstream  f(path, std::ios::in | std::ios::out);

  Practical note about appending:
    app forces the write position to the end before every write. If you need
    to write at arbitrary positions, don't use app.
*/

// ------------------------------------------------------------
// 3) Error handling strategies
// ------------------------------------------------------------
/*
  Streams do NOT throw exceptions by default.

  Strategy A: manual checks (simple, common)
    if (!f) { ... }
    if (!f.is_open()) { ... }
    if (f.fail()) { ... }

  Strategy B: exceptions (nice for libraries)
    f.exceptions(std::ios::failbit | std::ios::badbit);
    try { ... } catch (const std::ios_base::failure&) { ... }

  When do fail/bad happen?
    - failbit: formatted extraction couldn't parse, or some operation failed
    - badbit: low-level I/O error (rare but serious)
    - eofbit: attempted to read past end of file
*/

static std::string stream_state_to_string(std::ios const& s) {
    std::string out;
    if (s.good())
        out += "good ";
    if (s.eof())
        out += "eof ";
    if (s.fail())
        out += "fail ";
    if (s.bad())
        out += "bad ";
    if (out.empty())
        out = "(unknown state)";
    return out;
}

// ------------------------------------------------------------
// 4) Writing text
// ------------------------------------------------------------
/*
  Text output uses operator<< (formatted insertion) and manipulators:
    - std::fixed, std::scientific
    - std::setprecision(n)
    - std::setw(w)
    - std::left / std::right

  Newlines:
    - Use '\n' for plain newline.
    - std::endl writes '\n' AND flushes the stream (often slower). Use it
      only when you intentionally need a flush.
*/

static void write_text_demo(const fs::path& out_path) {
    std::ofstream out(out_path);
    if (!out) {
        throw std::runtime_error("Failed to open for writing: " + out_path.string());
    }

    out << "# Text file demo\n";
    out << "name,age,height_m\n";
    out << "Ada,36," << std::fixed << std::setprecision(2) << 1.65 << "\n";
    out << "Bjarne,73," << std::fixed << std::setprecision(2) << 1.78 << "\n";
    out << "\n";
    out << "Alignment demo:\n";
    out << std::left << std::setw(12) << "item" << std::right << std::setw(8) << "count" << "\n";
    out << std::left << std::setw(12) << "apple" << std::right << std::setw(8) << 12 << "\n";
    out << std::left << std::setw(12) << "banana" << std::right << std::setw(8) << 3 << "\n";
}

// ------------------------------------------------------------
// 5) Reading text: token-based vs line-based
// ------------------------------------------------------------
/*
  Token-based (formatted) extraction:
    int x; file >> x;
  - Skips leading whitespace.
  - Stops at whitespace.
  - Sets failbit if parsing fails.

  Line-based extraction:
    std::string line; std::getline(file, line);
  - Reads until '\n' (newline removed).
  - Works with empty lines.

  Common pitfall: mixing >> with getline
    After `file >> x;` the newline remains in the buffer; a following
    getline will read the rest of that line (possibly empty).

  Fix:
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
*/

static std::vector<std::string> read_all_lines(const fs::path& in_path) {
    std::ifstream in(in_path);
    if (!in) {
        throw std::runtime_error("Failed to open for reading: " + in_path.string());
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }
    return lines;
}

static std::string read_entire_file_to_string(const fs::path& in_path) {
    // Efficient, simple: stream buffer iterators.
    std::ifstream in(in_path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open: " + in_path.string());
    }
    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return data;
}

// ------------------------------------------------------------
// 6) Appending and atomic-ish write patterns
// ------------------------------------------------------------
/*
  Appending is easy:
    std::ofstream out(path, std::ios::app);

  Safer update pattern (commonly used for config files):
    - Write to a temp file in the same directory
    - Flush and close
    - Rename temp to target
  This reduces the risk of leaving a partially written file if the program
  crashes midway.

  NOTE: True atomicity depends on OS and filesystem behavior.
*/

static void append_line(const fs::path& path, std::string_view line) {
    std::ofstream out(path, std::ios::app);
    if (!out)
        throw std::runtime_error("Failed to open for append: " + path.string());
    out << line << '\n';
}

static void safe_replace_file(const fs::path& path, std::string_view contents) {
    fs::path tmp = path;
    tmp += ".tmp";

    {
        std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
        if (!out)
            throw std::runtime_error("Failed to open temp file: " + tmp.string());
        out.write(contents.data(), static_cast<std::streamsize>(contents.size()));
        out.flush();
        if (!out)
            throw std::runtime_error("Failed while writing temp file: " + tmp.string());
    }

    std::error_code ec;
    fs::rename(tmp, path, ec);
    if (ec) {
        // On Windows, rename-to-existing can fail; fallback is remove + rename.
        fs::remove(path, ec);
        ec.clear();
        fs::rename(tmp, path, ec);
    }
    if (ec) {
        // Best effort cleanup.
        fs::remove(tmp);
        throw std::system_error(ec, "Failed to replace file: " + path.string());
    }
}

// ------------------------------------------------------------
// 7) Binary I/O fundamentals
// ------------------------------------------------------------
/*
  Binary I/O uses read/write on char buffers:

    out.write(reinterpret_cast<const char*>(data), size);
    in.read(reinterpret_cast<char*>(data), size);

  IMPORTANT: Binary file format design matters.
    - Endianness: little vs big endian
    - Type sizes: int32_t vs long (platform dependent)
    - Struct padding/alignment changes layout
    - Floating point portability and NaN payloads

  Recommendation:
    - Use fixed-width integers: std::uint32_t, std::int64_t, etc.
    - Define a file format (magic bytes + version)
    - Serialize fields explicitly rather than dumping raw structs
*/

template <class T> static void write_pod(std::ostream& out, const T& value) {
    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
    out.write(reinterpret_cast<const char*>(&value), static_cast<std::streamsize>(sizeof(T)));
}

template <class T> static void read_pod(std::istream& in, T& value) {
    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
    in.read(reinterpret_cast<char*>(&value), static_cast<std::streamsize>(sizeof(T)));
}

static void write_binary_demo(const fs::path& bin_path) {
    std::ofstream out(bin_path, std::ios::binary | std::ios::trunc);
    if (!out)
        throw std::runtime_error("Failed to open for binary write: " + bin_path.string());

    // A tiny custom binary format:
    //   magic: 4 bytes "IOG1" (I/O Guide v1)
    //   count: uint32
    //   repeated (count times): int32
    const std::array<char, 4> magic{{'I', 'O', 'G', '1'}};
    out.write(magic.data(), static_cast<std::streamsize>(magic.size()));

    const std::vector<std::int32_t> values{1, -2, 300000, 42};
    std::uint32_t count = static_cast<std::uint32_t>(values.size());
    write_pod(out, count);
    for (std::int32_t v : values)
        write_pod(out, v);

    out.flush();
    if (!out)
        throw std::runtime_error("Binary write failed: " + bin_path.string());
}

static std::vector<std::int32_t> read_binary_demo(const fs::path& bin_path) {
    std::ifstream in(bin_path, std::ios::binary);
    if (!in)
        throw std::runtime_error("Failed to open for binary read: " + bin_path.string());

    std::array<char, 4> magic{};
    in.read(magic.data(), static_cast<std::streamsize>(magic.size()));
    if (!in)
        throw std::runtime_error("Failed to read magic: " + bin_path.string());
    if (!(magic[0] == 'I' && magic[1] == 'O' && magic[2] == 'G' && magic[3] == '1')) {
        throw std::runtime_error("Bad magic (not an IOG1 file): " + bin_path.string());
    }

    std::uint32_t count = 0;
    read_pod(in, count);
    if (!in)
        throw std::runtime_error("Failed to read count: " + bin_path.string());

    std::vector<std::int32_t> values;
    values.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        std::int32_t v = 0;
        read_pod(in, v);
        if (!in)
            throw std::runtime_error("Failed to read value index " + std::to_string(i));
        values.push_back(v);
    }
    return values;
}

// ------------------------------------------------------------
// 8) Random access: seek/tell
// ------------------------------------------------------------
/*
  Use seekg/tellg for input position; seekp/tellp for output position.

  - tellg() returns current input position (or -1 on failure)
  - seekg(pos) sets input position

  Useful for:
    - scanning fixed-size records
    - reading a file footer/header
    - resuming reading from a checkpoint

  Text mode seeking can be tricky on some platforms due to newline translation.
  For reliable byte offsets, open in binary mode.
*/

static std::string read_last_n_bytes(const fs::path& path, std::size_t n) {
    std::ifstream in(path, std::ios::binary);
    if (!in)
        throw std::runtime_error("Failed to open: " + path.string());

    in.seekg(0, std::ios::end);
    std::streampos end = in.tellg();
    if (end < 0)
        throw std::runtime_error("tellg failed: " + path.string());

    std::size_t size = static_cast<std::size_t>(end);
    std::size_t start = (n >= size) ? 0 : (size - n);
    in.seekg(static_cast<std::streamoff>(start), std::ios::beg);

    std::string buf(size - start, '\0');
    in.read(buf.data(), static_cast<std::streamsize>(buf.size()));
    if (!in && !in.eof()) {
        throw std::runtime_error("read failed: " + path.string() +
                                 " state=" + stream_state_to_string(in));
    }
    return buf;
}

// ------------------------------------------------------------
// 9) Stream states and robust read loops
// ------------------------------------------------------------
/*
  Prefer idiomatic loops:
    while (std::getline(in, line)) { ... }

  Avoid:
    while (!in.eof()) { std::getline(in, line); ... }
  because eofbit is only set after an attempted read past EOF.

  After a failed formatted read, you can:
    - clear() to reset flags
    - ignore() to skip bad input

  Example (parsing ints with fallback):
    int x;
    if (!(in >> x)) { in.clear(); in.ignore(...); }
*/

static std::vector<int> parse_ints_from_text_file(const fs::path& path) {
    std::ifstream in(path);
    if (!in)
        throw std::runtime_error("Failed to open: " + path.string());

    std::vector<int> xs;
    int x;
    while (true) {
        if (in >> x) {
            xs.push_back(x);
            continue;
        }
        if (in.eof())
            break;
        // Some non-integer token: recover by skipping one token.
        in.clear();
        std::string junk;
        in >> junk;
        if (!in)
            break;
    }
    return xs;
}

// ------------------------------------------------------------
// 10) Performance notes (practical)
// ------------------------------------------------------------
/*
  - iostreams are buffered by default.
  - Avoid flushing often (std::endl, std::flush) unless needed.
  - For huge files, read in larger blocks (binary) or use iterators.
  - If you also use std::cin/std::cout heavily, and not mixing with C stdio,
    you can speed up console I/O (not file I/O) with:
      std::ios::sync_with_stdio(false);
      std::cin.tie(nullptr);

  Buffer control (advanced):
    You can use rdbuf()->pubsetbuf(...) but the benefits vary and it's rarely
    needed for typical learning projects.
*/

// ------------------------------------------------------------
// 11) CSV-ish parsing example (simple, not a full CSV parser)
// ------------------------------------------------------------
/*
  Real CSV parsing is deceptively tricky (quotes, escapes, commas in fields).
  For learning, a simple split on ',' can be OK for controlled input.
*/

static std::vector<std::string> split_simple(std::string_view s, char delim) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (start <= s.size()) {
        std::size_t pos = s.find(delim, start);
        if (pos == std::string_view::npos)
            pos = s.size();
        parts.emplace_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return parts;
}

static void read_text_demo(const fs::path& text_path) {
    std::vector<std::string> lines = read_all_lines(text_path);
    std::cout << "Read " << lines.size() << " lines from " << text_path << "\n";

    // Print the header and one parsed row.
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].empty())
            continue;
        if (lines[i][0] == '#')
            continue;
        if (lines[i].rfind("name", 0) == 0) {
            std::cout << "Header: " << lines[i] << "\n";
            continue;
        }
        auto cols = split_simple(lines[i], ',');
        if (cols.size() == 3) {
            std::cout << "Row parsed: name=" << cols[0] << " age=" << cols[1]
                      << " height=" << cols[2] << "\n";
            break;
        }
    }
}

// ------------------------------------------------------------
// 12) Exceptions mode demo
// ------------------------------------------------------------
static std::optional<std::string> read_first_line_exceptions(const fs::path& path) {
    std::ifstream in;
    in.exceptions(std::ios::failbit | std::ios::badbit);
    try {
        in.open(path);
        std::string line;
        if (!std::getline(in, line))
            return std::nullopt;
        return line;
    } catch (const std::ios_base::failure& e) {
        // In real code you might propagate, map to your error type, etc.
        std::cerr << "I/O exception while reading " << path << ": " << e.what() << "\n";
        return std::nullopt;
    }
}

// ------------------------------------------------------------
// 13) A tiny checklist (what to do in real projects)
// ------------------------------------------------------------
/*
  Text config/log files:
    - Use std::ifstream/std::ofstream
    - Use std::getline for line-by-line parsing
    - Validate inputs; don't trust formats
    - Write updates via temp + rename for safety

  Binary formats:
    - Design a file format: magic + version + length fields
    - Use fixed-width integer types
    - Serialize explicitly; avoid writing raw structs
    - Consider endianness (define it; commonly little-endian)

  Always:
    - Check open success
    - Handle errors (return error, throw exception, log + exit)
    - Avoid EOF-driven loops
*/

// ------------------------------------------------------------
// Main: runs the demos and prints a few results.
// ------------------------------------------------------------
int main() {
    try {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);

        const fs::path out_dir = "io_guide_output";
        std::error_code ec;
        fs::create_directories(out_dir, ec);
        if (ec)
            throw std::system_error(ec, "Failed to create output directory");

        const fs::path text_path = out_dir / "people.txt";
        const fs::path bin_path = out_dir / "values.iog";
        const fs::path replace_path = out_dir / "config.txt";

        write_text_demo(text_path);
        append_line(text_path, "# Appended line");

        // Demonstrate reading an entire file into memory.
        {
            std::string whole = read_entire_file_to_string(text_path);
            std::cout << "Loaded " << whole.size() << " bytes from " << text_path << "\n";
        }

        read_text_demo(text_path);
        std::cout << "Last 24 bytes of " << text_path << ":\n";
        std::cout << read_last_n_bytes(text_path, 24) << "\n";

        write_binary_demo(bin_path);
        auto values = read_binary_demo(bin_path);
        std::cout << "Binary values read (" << values.size() << "): ";
        for (std::size_t i = 0; i < values.size(); ++i) {
            std::cout << values[i] << (i + 1 == values.size() ? "" : ", ");
        }
        std::cout << "\n";

        // Demonstrate "safe replace".
        safe_replace_file(replace_path, "mode=demo\nretries=3\n");
        safe_replace_file(replace_path, "mode=demo\nretries=4\n");
        auto first = read_first_line_exceptions(replace_path);
        if (first)
            std::cout << "First line of " << replace_path << ": " << *first << "\n";

        // Parse ints demo: create a file with mixed tokens.
        const fs::path ints_path = out_dir / "mixed_ints.txt";
        safe_replace_file(ints_path, "10 20 nope 30\n40 forty 50\n");
        auto xs = parse_ints_from_text_file(ints_path);
        std::cout << "Parsed ints from mixed file: ";
        for (std::size_t i = 0; i < xs.size(); ++i) {
            std::cout << xs[i] << (i + 1 == xs.size() ? "" : " ");
        }
        std::cout << "\n";

        std::cout << "\nCreated demo files in: " << out_dir << "\n";
        std::cout << "- " << text_path << "\n";
        std::cout << "- " << bin_path << "\n";
        std::cout << "- " << replace_path << "\n";
        std::cout << "- " << ints_path << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
