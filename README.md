# codestat-web

> A full-featured C-based and AI-enhanced codebase analysis tool with JSON reporting.

---

## Overview

**codestat-web** is a powerful, extensible, multi-language code analysis utility designed to scan and report critical metrics from codebases, especially in **C** and **Java**. The tool walks through directories, identifies and analyzes relevant files, computes various code metrics, and generates structured output in JSON format suitable for dashboards, automation, or further AI processing.

This utility is ideal for developers, software architects, and CI/CD pipelines where understanding the quality, structure, and maintainability of a codebase is essential.

---

## Features

- 📂 **Recursive File Scanning**  
  Automatically locates all `.c`, `.h`, and `.java` files within a given directory structure.

- 📊 **Metrics Analysis per File**  
  For each file, codestat calculates:
  - Total lines of code
  - Number of code lines vs. blank lines
  - Number of functions
  - **Blast radius** estimation (how risky it is to delete or change the file)
  - **Worst-case time complexity**, powered by optional AI logic

- 🧠 **AI Integration (Optional)**  
  Uses a Python script (`ai_complexity.py`) to analyze the content of each function and estimate time complexity intelligently.

- 🧾 **Structured JSON Output**  
  All results are output to `report.json` in JSON array format, including a summary section.

- 🔍 **Interactive CLI Mode (Optional)**  
  Includes a command loop (can be re-enabled) for querying individual files interactively using commands like `scanfile <filename>`.

- ⚙️ **Modular Design**  
  All analysis logic is separated across modules like `loc.c`, `complexity.c`, `function_count.c`, and `analyzer.c`, making it easy to extend.

---

## Directory Structure

```
codestat-web/
├── codestat-runall/         # Core source code folder
│   ├── src/
│   │   ├── main.c
│   │   ├── scanner.c/h
│   │   └── analyzer/
│   │       ├── analyzer.c/h
│   │       ├── loc.c/h
│   │       ├── complexity.c/h
│   │       ├── function_count.c/h
│   │       └── comment_todo.c/h
│   ├── Makefile
│   └── report.json          # Auto-generated analysis output
├── ai_complexity.py         # Optional AI Python script for complexity analysis
└── venv/                    # Optional Python virtual environment
```

---

## Getting Started

### Prerequisites

- GCC (for compiling the C code)
- Python 3.10+ (if using AI analysis)
- Git (for version control)

### Installation

Clone the repo:

```bash
git clone https://github.com/RayanRadi/codestat-web.git
cd codestat-web/codestat-runall
```

Build the tool:

```bash
make
```

(Optional) Set up the Python environment:

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

---

## Usage

### Run analysis on a directory

```bash
./codestat-runall Test-Subject/
```

This will:

- Recursively scan all valid files in the `Test-Subject/` directory
- Generate `report.json` in the same folder

### Sample JSON Output

```json
[
  {
    "file": "Test-Subject/example.c",
    "total_lines": 200,
    "code_lines": 150,
    "blank_lines": 20,
    "functions": 6,
    "blast_radius": "Medium",
    "worst_complexity": "O(n^2)"
  },
  {
    "summary": {
      "total_loc": 420,
      "files_scanned": 3
    }
  }
]
```

---

## Understanding the Metrics

- **Lines of Code (LOC)**: Total vs. code vs. blank lines.
- **Functions**: Number of functions defined in the file.
- **Blast Radius**: An estimate of the potential impact if this file were modified or deleted.
  - *Low*: < 5 functions
  - *Medium*: 5–9 functions
  - *High*: 10+ functions
- **Worst Complexity**: Either statically estimated or returned by the `ai_complexity.py` script by analyzing function structure.

---

## AI Complexity (Optional)

To use the AI-powered time complexity estimator:
1. Ensure `ai_complexity.py` is located at the path hardcoded in `complexity.c`
2. Modify the path in `call_ai_on_function()` if necessary:
```c
/usr/bin/python3 /full/path/to/ai_complexity.py temp_func.txt
```
3. The script must return a single line like `O(n)` or `O(n^2)` for each function body.

---

## Tagging Releases

To tag a stable version:

```bash
git tag v1.0.0
git push origin v1.0.0
```

---

## Development Roadmap

Potential future additions:
- Language support beyond C/Java (Python, JS, etc.)
- JSON schema validation
- CI integration (GitHub Actions, etc.)
- Dockerized build for portable analysis

---

## Contributing

This project is currently maintained individually. If you'd like to contribute, feel free to fork, make changes, and submit a pull request with a clear description of the improvements or bug fixes.

---

## License

This project is licensed under the MIT License.

---

## Author

**Rayan Radi**  
[https://github.com/RayanRadi](https://github.com/RayanRadi)
