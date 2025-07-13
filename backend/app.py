from flask import Flask, request, jsonify
from flask_cors import CORS
import os
import zipfile
import subprocess
import glob
import shutil

app = Flask(__name__)
CORS(app)

@app.route("/analyze", methods=["POST"])
def analyze():
    # Save uploaded zip file
    zip_file = request.files['file']
    zip_path = "upload.zip"
    zip_file.save(zip_path)

    # Extract to temp_code/, removing old contents
    extract_folder = "temp_code"
    if os.path.exists(extract_folder):
        shutil.rmtree(extract_folder)
    os.makedirs(extract_folder)

    with zipfile.ZipFile(zip_path, 'r') as zip_ref:
        zip_ref.extractall(extract_folder)

    # Run codestat-runall on uploaded code
    binary_path = "src/codestat-runall"  # Adjust path if needed
    full_command = f'"{binary_path}" "{extract_folder}"'
    subprocess.run(full_command, shell=True)

    # Read codestat report
    try:
        with open("report.json", "r") as f:
            report_content = f.read()
    except FileNotFoundError:
        return jsonify({"success": False, "error": "report.json not found."})

    # Optional: run AI complexity on each file
    ai_results = {}
    for code_file in glob.glob(f"{extract_folder}/**/*.c", recursive=True) + \
                    glob.glob(f"{extract_folder}/**/*.h", recursive=True):
        try:
            ai_output = subprocess.check_output(
                ["python3", "backend/ai_complexity.py", code_file],
                stderr=subprocess.STDOUT,
                timeout=60
            ).decode().strip()
            ai_results[code_file] = ai_output
        except subprocess.CalledProcessError:
            ai_results[code_file] = "AI_ERROR"
        except subprocess.TimeoutExpired:
            ai_results[code_file] = "AI_TIMEOUT"

    return jsonify({
        "success": True,
        "report": report_content,
        "ai_complexities": ai_results
    })

# ✅ Needed for deployment
from waitress import serve

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 10000))
    serve(app, host="0.0.0.0", port=port)
