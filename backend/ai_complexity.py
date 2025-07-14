import sys
import os
import requests
import re
import threading
import time
import signal

signal.signal(signal.SIGPIPE, signal.SIG_DFL)

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
    token = os.getenv("OPENROUTER_API_KEY")
    if not token:
        print("Error: OPENROUTER_API_KEY environment variable not set.")
        return "AI_ERROR"

    headers = {
        "Authorization": f"Bearer {token}",
        "Content-Type": "application/json"
    }

    prompt = (
        "You are an expert C programmer. Analyze this C code and return the single worst-case time complexity "
        "in Big-O notation only. Respond only with: O(1), O(n), O(n^2), O(n log n), O(2^n), etc.\n\n"
        f"{function_code}\n\n"
        "Only return the worst-case Big-O. No explanation."
    )

    data = {
        "model": "mistralai/mistral-7b-instruct",
        "messages": [
            {"role": "system", "content": "You are a code analysis expert."},
            {"role": "user", "content": prompt}
        ]
    }

    spinner = Spinner()
    spinner.start()

    try:
        response = requests.post(
            "https://openrouter.ai/api/v1/chat/completions",
            headers=headers,
            json=data,
            timeout=60
        )
        spinner.stop()
        response.raise_for_status()

        content = response.json()["choices"][0]["message"]["content"]
        matches = re.findall(r"O\([^)]+\)", content)
        if matches:
            return matches[-1].strip()
        else:
            return "AI_ERROR"

    except requests.exceptions.Timeout:
        spinner.stop()
        print("Error: OpenRouter API timed out.")
        return "AI_TIMEOUT"
    except Exception as e:
        spinner.stop()
        print(f"Error calling OpenRouter API: {e}")
        return "AI_ERROR"

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: ai_complexity.py <function_file>")
        sys.exit(1)

    func_code = read_func(sys.argv[1])
    result = ask_ai(func_code)
    print(result)
