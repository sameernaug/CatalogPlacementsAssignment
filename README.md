# Catalog Placements Secret‑Sharing

A C++ solution that recovers the constant term (`c`) of an unknown polynomial via Lagrange interpolation, given a set of base‑encoded points in JSON files.

---

## 📖 Problem Statement

You are given one or more JSON files, each containing:

- **keys**: `{ "n": <total points>, "k": <threshold> }`
- **Point entries**: each key is an integer `x`, and its value is an object:
  ```jsonc
  "<x>": {
    "base": "<base>",
    "value": "<encoded y>"
  }
  ```

Each point represents \`\`\` (x, y) where y = (value string) interpreted in the given base. \`\`\`

Your task:

1. Read all points from each JSON.
2. Decode each `value` into a decimal integer (`boost::multiprecision::cpp_int`).
3. Select any `k` distinct points (the code sorts them by `x` and takes the first `k`).
4. Use **Lagrange interpolation** to compute the secret constant term \(c = f(0)\) of the polynomial.
5. Print the recovered secrets for both test cases in one run.

---

## 🛠️ Approach

1. **Parsing**

   - Load the JSON via [nlohmann/json].
   - Extract `n`, `k`, and each point’s `x`, `base`, `value`.

2. **Base conversion**

   - Reverse‑iterate the `value` string.
   - For each character, convert to digit (0–15) and accumulate with a running power of `base` into a `cpp_int`.

3. **Point selection**

   - Store all `(x, y)` pairs in a `vector`.
   - Sort by `x` and take the first `k` entries.

4. **Lagrange interpolation**

   - We want \(c = f(0) = \sum_{i=0}^{k-1} y_i \prod_{j\ne i} \frac{0 - x_j}{x_i - x_j}.\)
   - In code, for each i:
     ```cpp
     numerator = ∏_{j≠i} (0 - x_j);
     denominator = ∏_{j≠i} (x_i - x_j);
     term = y_i * numerator / denominator;
     secret += term;
     ```
   - Integer division is exact by problem guarantees.

5. **Output**

   - Print `Secret for testcase1.json: <c1>` and `Secret for testcase2.json: <c2>`.

---

## 📂 File Structure

```
project-root/
├── include/
│   └── nlohmann/json.hpp      # JSON library header
├── src/
│   └── main.cpp               # Your solution code
├── testcase1.json             # First test case
├── testcase2.json             # Second test case
└── README.md                  # This document
```

---

## ⚙️ Dependencies

- **Compiler**: `g++` or compatible (C++17)
- **Boost.Multiprecision** (header‑only): for `cpp_int` arbitrary‑precision integers
- **nlohmann/json** (header‑only): for JSON parsing

---

## 🏗️ Build & Run

From the project root:

```bash
# Compile
g++ src/main.cpp -std=c++17 -O2 -I./include \
    -o secret

# Execute
./secret testcase1.json testcase2.json
```

---

## 📝 Explanation of Key Components

### 1. `base_to_decimal` function

- Converts a string in base 2–16 to a `cpp_int`.
- Processes characters from LSD to MSD, multiplying an accumulator by the base each step.

### 2. JSON parsing & point collection

- Skips the top‑level `"keys"` entry.
- Converts each JSON key (string) to `x` and its `value` to `y`.

### 3. Lagrange interpolation (compute\_secret)

- For each of the first `k` points, forms a term whose numerator is the product of all `-x_j` (j≠i) and denominator is the product of `(x_i−x_j)`.
- Division yields an exact integer term, accumulated into `secret`.

---

*(This README describes how your own C++ solution implements the required steps from the assignment.)*

