# Catalog Placements Secret‑Sharing

A C++ solution that recovers the constant term (`c`) of an unknown polynomial via Lagrange interpolation, given a set of base‑encoded points in JSON files.

---

## 📖 Problem Statement

You are given one or more JSON files, each containing:

* **keys**: `{ "n": <total points>, "k": <threshold> }`
* **Point entries**: each key is an integer `x`, and its value is an object:

  ```jsonc
  "<x>": {
    "base": "<base>",
    "value": "<encoded y>"
  }
  ```

Each point represents

```text
(x, y) where y = (value string) interpreted in the given base.
```

Your task:

1. Read all points from each JSON.
2. Decode each `value` into a decimal integer (`boost::multiprecision::cpp_int`).
3. Select any `k` distinct points (the code sorts them by `x` and takes the first `k`).
4. Use **Lagrange interpolation** to compute the secret constant term $c = f(0)$ of the polynomial.
5. Print the recovered secrets for both test cases in one run.

---

## 🛠️ Approach & Solution Steps

1. **Understand sample format**

   * Confirm JSON structure and how `base` + `value` encode each $y$.
   * Decide to treat each JSON key as the $x$-coordinate and decode to big integers.

2. **JSON parsing**

   * Use \[nlohmann/json] to load each file into a `json` object.
   * Extract `n` and `k` from `data["keys"]`.
   * Iterate over all entries, skip the `"keys"` object, and collect `(x, base, value_str)`.

3. **Base conversion** (`base_to_decimal`)

   * For each character in `value_str` (from least to most significant):

     * Map '0'–'9','A'–'F' to digit values (0–15).
     * Multiply a running `power` by `base` each iteration.
     * Accumulate `digit * power` into a `cpp_int` result.
   * This yields the decimal big‑integer $y$.

4. **Selecting points**

   * Store all `(x, y)` in a `vector<pair<cpp_int, cpp_int>>`.
   * Sort by `x`.
   * Take the first `k` entries (any `k` would work; sorting is deterministic).

5. **Lagrange interpolation** (`compute_secret`)

   * Goal: compute $c = f(0) = \sum_{i=0}^{k-1} y_i \prod_{j\neq i} \frac{0 - x_j}{x_i - x_j}$.
   * For each index `i`:

     1. `numerator = ∏_{j≠i} (0 - x_j)`
     2. `denominator = ∏_{j≠i} (x_i - x_j)`
     3. Multiply `points[i].second * numerator`, then divide by `denominator` (exact integer division).
     4. Add the result to `secret`.

6. **Testing & verification**

   * Compile and run locally on the provided `testcase1.json` and `testcase2.json`.
   * Cross‑check a small sample manually (e.g., 3‑point quadratic) to ensure `c` matches.

7. **Output**

   * Print:

     ```text
     Secret for testcase1.json: <c1>
     Secret for testcase2.json: <c2>
     ```

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

* **Compiler**: `g++` or compatible (C++17)
* **Boost.Multiprecision** (header‑only): for `cpp_int` arbitrary‑precision integers
* **nlohmann/json** (header‑only): for JSON parsing

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

* Converts a string in base 2–16 to a `cpp_int`.
* Processes characters from LSD to MSD, multiplying an accumulator by the base each step.

### 2. JSON parsing & point collection

* Skips the top‑level `"keys"` entry.
* Converts each JSON key (string) to `x` and its `value` to `y`.

### 3. Lagrange interpolation (`compute_secret`)

* For each of the first `k` points, forms a term whose numerator is the product of all `-x_j` (j≠i) and denominator is the product of `(x_i−x_j)`.
* Exact integer division yields each term, which are summed into `secret`.

---

*(This README describes how your own C++ solution implements the required steps from the assignment.)*
