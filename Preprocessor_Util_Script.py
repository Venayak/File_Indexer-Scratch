# CONVERT TO .EXE FOR 0 USER SETUP!
# python -m PyInstaller --onefile Preprocessor_Util_Script.py

from docx import Document
from PyPDF2 import PdfReader
import sys
import os


def docx_to_txt(input_file):
    doc = Document(input_file)
    text = []

    for para in doc.paragraphs:
        text.append(para.text)

    output_file = os.path.splitext(input_file)[0] + ".txt"

    with open(output_file, "w", encoding="utf-8") as f:
        f.write("\n".join(text))


def pdf_to_txt(input_file):
    reader = PdfReader(input_file)
    text = []

    for page in reader.pages:
        content = page.extract_text()
        if content:
            text.append(content)

    output_file = os.path.splitext(input_file)[0] + ".txt"

    with open(output_file, "w", encoding="utf-8") as f:
        f.write("\n".join(text))

if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Usage: script <filename>")
        sys.exit(1)

    input_file = sys.argv[1]

    ext = os.path.splitext(input_file)[1].lower()

    if ext == ".docx":
        docx_to_txt(input_file)
    elif ext == ".pdf":
        pdf_to_txt(input_file)
    else:
        print("Unsupported file format")