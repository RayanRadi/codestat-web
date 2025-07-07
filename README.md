# codestat-web

> A full-stack, AI-assisted codebase analysis tool featuring a command-line engine, C/Python backend, and web-based frontend interface.

---

## Overview

**codestat-web** is a powerful and extensible full-stack utility for analyzing source code across directories. It features:

- A C-based command-line analysis engine (`codestat-runall`)
- A Python (Flask) backend for routing, control, and AI integration
- A web-based frontend for interactive use

The system recursively scans C, header, and Java source files, calculates structural metrics, estimates time complexity using AI, and displays results in both machine-readable JSON and a visual HTML dashboard.

---

## Features

### CLI Engine (`codestat-runall`)
- Recursive file scanning (`.c`, `.h`, `.java`)
- Line-of-code (LOC) statistics
- Function counting
- Worst-case time complexity estimation
- Blast radius labeling
- JSON report generation

### Web Backend (Python Flask)
- Exposes analysis endpoints via REST API
- Integrates AI via `ai_complexity.py`
- Launches analysis and serves output to frontend

### Web Frontend (HTML/JS)
- Clean GUI to trigger analysis
- View results visually from JSON reports
- User input and feedback support

---

## Directory Structure

```
codestat-web/
├── codestat-runall/         # C-based command-line engine
│   ├── src/
│   │   ├── main.c, scanner.c/h
│   │   └── analyzer/
│   │       ├── analyzer.c/h, loc.c/h, complexity.c/h, etc.
│   ├── Makefile
│   └── report.json
├── backend/                 # Python Flask backend
│   ├── app.py
│   └── ai_complexity.py     # AI function analysis
├── frontend/                # HTML/JS frontend
│   └── index.html, style.css, script.js (etc.)
├── venv/                    # Optional Python virtual environment
└── README.md
```

---

## Installation & Setup

### 1. Clone the Repository

```bash
git clone https://github.com/RayanRadi/codestat-web.git
cd codestat-web
```

### 2. Build the CLI engine

```bash
cd codestat-runall
make
cd ..
```

### 3. Set up the backend

```bash
python3 -m venv venv
source venv/bin/activate
pip install flask
```

---

## Usage

### Command-Line Analysis (CLI)

Run the CLI engine on a code directory:

```bash
./codestat-runall Test-Subject/
```

This generates a `report.json` file with metrics for all scanned files.

### Web Interface

Start the backend server:

```bash
cd backend
source ../venv/bin/activate
python app.py
```

Then open the web app:

```
http://localhost:5000/
```

Use the frontend GUI to trigger scans and view results visually.

---

## Sample JSON Output

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

## Metrics Explained

- **Lines of Code (LOC)**: Total, code, and blank lines.
- **Functions**: Counted using basic parsing.
- **Blast Radius**: Categorized as:
  - Low: <5 functions
  - Medium: 5–9 functions
  - High: 10+ functions
- **Worst Complexity**: Based on loop analysis or external AI via `ai_complexity.py`.

---


## Roadmap (Future Improvements)

- Language support: Python, JavaScript, more
- Real-time frontend refresh on analysis
- Graphical metrics dashboard
- Docker support
- GitHub CI workflow

---


## Author

**Rayan Radi**  
https://github.com/RayanRadi
