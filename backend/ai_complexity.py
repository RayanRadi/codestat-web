import sys
import os
import requests
import re
import threading
import time
import signal

# Handle broken pipe errors silently (final boss patch)
signal.signal(signal.SIGPIPE, signal.SIG_DFL)

# Spinner class with error-safe printing
class Spinner:
    def __init__(self, message="Analyzing with AI... "):
        self.message = message
        self.stop_flag = False
        self.thread = threading.Thread(target=self._spin)

    def _spin(self):
        spin_chars = "|/-\\"
        i = 0
        try:
            print(self.message, end="", flush=True)
            while not self.stop_flag:
                try:
                    print(spin_chars[i % len(spin_chars)], end="\r", flush=True)
                    time.sleep(0.1)
                    i += 1
                except BrokenPipeError:
                    break
        except Exception:
            pass

    def start(self):
        self.thread.start()

    def stop(self):
        self.stop_flag = True
        self.thread.join()
        try:
            print(" " * (len(self.message) + 2), end="\r")
        except BrokenPipeError:
            pass


def read_func(path):
    with open(path, 'r') as f:
        return f.read()

def ask_ai(function_code):
    token = os.getenv("HF_TOKEN")
    if not token:
        print("Error: HF_TOKEN environment variable not set.")
        return "AI_ERROR"

    headers = {
        "Authorization": f"Bearer {token}",
        "Content-Type": "application/json"
    }

    prompt = (
        "This is a C code file. Analyze all functions and return the single worst-case time complexity "
        "among them. Respond with only the Big-O notation, like: 'O(1)', 'O(n)', 'O(n^2)', 'O(n log n)', or 'O(2^n)'.\n\n"
        f"{function_code}\n\n"
        "Only return the worst-case Big-O. No explanation."
    )

    data = {
        "inputs": prompt
    }

    spinner = Spinner()
    spinner.start()

    try:
        response = requests.post(
            "https://api-inference.huggingface.co/models/HuggingFaceH4/zephyr-7b-beta",
            headers=headers,
            json=data,
            timeout=60
        )
        spinner.stop()
        response.raise_for_status()
        raw = response.json()[0]["generated_text"]
        matches = re.findall(r"O\([^)]+\)", raw)
        if matches:
            clean = matches[-1].strip()
            if not re.match(r"O\(\s*(1|n|n\^2|n log n|2\^n)\s*\)", clean):
                return "O(n)"
            return clean
        else:
            return "AI_ERROR"
    except requests.exceptions.Timeout:
        spinner.stop()
        print("Error: Hugging Face API timed out.")
        return "AI_TIMEOUT"
    except Exception as e:
        spinner.stop()
        print(f"Error calling Hugging Face API: {e}")
        return "AI_ERROR"

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: ai_complexity.py <function_file>")
        sys.exit(1)

    func_code = read_func(sys.argv[1])
    result = ask_ai(func_code)
    print(result)
