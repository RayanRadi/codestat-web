from flask import Flask, request, jsonify
from flask_cors import CORS
import os
import zipfile
import subprocess
import glob

app = Flask(__name__)
CORS(app)

@app.route("/analyze", methods=["POST"])
def analyze():
    # Save zip
    zip_file = request.files['file']
    zip_path = "upload.zip"
    zip_file.save(zip_path)

    # Unzip
    extract_folder = "temp_code"
    subprocess.run(["rm", "-rf", extract_folder])
    os.makedirs(extract_folder)

    with zipfile.ZipFile(zip_path, 'r') as zip_ref:
        zip_ref.extractall(extract_folder)

    # Run codestat
    binary_path = "/usr/local/bin/codestat-runall"
    full_command = f'"{binary_path}" "{extract_folder}"'
    subprocess.run(full_command, shell=True)

    # Load codestat report
    try:
        with open("report.json", "r") as f:
            report_content = f.read()
    except FileNotFoundError:
        return jsonify({"success": False, "error": "report.json not found."})

    # Run AI on each .c/.h file
    ai_results = {}
    for code_file in glob.glob(f"{extract_folder}/**/*.c", recursive=True) + \
                    glob.glob(f"{extract_folder}/**/*.h", recursive=True):
        try:
            ai_output = subprocess.check_output(
                ["python3", "ai_complexity.py", code_file],
                stderr=subprocess.STDOUT,
                timeout=60
            ).decode().strip()
            ai_results[code_file] = ai_output
        except subprocess.CalledProcessError as e:
            ai_results[code_file] = "AI_ERROR"
        except subprocess.TimeoutExpired:
            ai_results[code_file] = "AI_TIMEOUT"

    return jsonify({
        "success": True,
        "report": report_content,
        "ai_complexities": ai_results
    })

if __name__ == "__main__":
    app.run(debug=True)
