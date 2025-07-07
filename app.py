from flask import Flask, request, jsonify
from flask_cors import CORS

import os
import zipfile
import subprocess

app = Flask(__name__)
CORS(app)

@app.route("/analyze", methods=["POST"])
def analyze():
    # Save uploaded zip
    zip_file = request.files['file']
    zip_path = "upload.zip"
    zip_file.save(zip_path)

    # Unzip to temp_code/
    extract_folder = "temp_code"
    if os.path.exists(extract_folder):
        subprocess.run(["rm", "-rf", extract_folder])
    os.makedirs(extract_folder)

    with zipfile.ZipFile(zip_path, 'r') as zip_ref:
        zip_ref.extractall(extract_folder)

    # Run codestat-runall using full path + shell
    binary_path = "/usr/local/bin/codestat-runall"
    full_command = f'"{binary_path}" "{extract_folder}"'
    print("Running shell command:", full_command)
    result = subprocess.run(full_command, shell=True)

    # Return report.json content
    try:
        with open("report.json", "r") as f:
            report = f.read()
        return jsonify({"success": True, "report": report})
    except FileNotFoundError:
        return jsonify({"success": False, "error": "report.json not found."})

if __name__ == "__main__":
    app.run(debug=True)
