```python
import os
import subprocess
from pathlib import Path

# Configuration
OLLAMA_MODEL = "llama3.1"
MAX_FILE_SIZE_KB = 80  # Prevent large files
EXCLUDED_DIRS = {".git", "__pycache__", "node_modules", ".venv", "dist", "build"}
ALLOWED_EXTENSIONS = {
    ".py", ".js", ".ts", ".c", ".cpp", ".h", ".ino",
    ".java", ".md", ".txt", ".html", ".css", ".json"
}

# Utility functions

def is_valid_file(path: Path) -> bool:
    """Check if a file is valid (allowed extension and size)"""
    return path.suffix.lower() in ALLOWED_EXTENSIONS and path.stat().st_size <= MAX_FILE_SIZE_KB * 1024


def call_ollama(prompt: str) -> str:
    """Call Ollama to improve code and generate README.md"""
    process = subprocess.Popen(
        ["ollama", "run", OLLAMA_MODEL],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding="utf-8"
    )
    output, error = process.communicate(prompt)

    if process.returncode != 0:
        print("❌ Error Ollama:", error)
        return ""

    return output.strip()


def improve_code(file_path: Path):
    """Improve code and generate improved content"""
    original = file_path.read_text(encoding="utf-8", errors="ignore")

    prompt = f"""
You are a senior software engineer.

Improve the following file professionally:
- Keep the same functionality
- Improve readability and structure
- Add clear comments where needed
- Follow best practices
- Do NOT add explanations outside the code
- Return ONLY the improved file content

File: {file_path.name}

Content:
{original}
"""

    improved = call_ollama(prompt)

    if improved and improved != original:
        file_path.write_text(improved, encoding="utf-8")
        print(f"✅ Improved: {file_path}")


def generate_readme(repo_name: str, files_summary: str):
    """Generate a professional README.md for a GitHub repository"""
    prompt = f"""
You are a senior technical writer.

Create a professional README.md for a GitHub repository.

Repository name: {repo_name}

Repository contents summary:
{files_summary}

README requirements:
- Clear project description
- Features
- Installation
- Usage
- Project structure
- Requirements
- Author section
- Professional tone
- Markdown format
"""

    return call_ollama(prompt)


def summarize_repo(files):
    """Summarize repository contents"""
    summary = ""
    for f in files:
        summary += f"- {f}\n"
    return summary


# Main function

def main():
    repo_root = Path(".").resolve()
    repo_name = repo_root.name

    editable_files = []

    for root, dirs, files in os.walk(repo_root):
        dirs[:] = [d for d in dirs if d not in EXCLUDED_DIRS]
        for file in files:
            path = Path(root) / file
            if is_valid_file(path):
                editable_files.append(path)

    print(f"📂 Files to process: {len(editable_files)}")

    for file_path in editable_files:
        if file_path.name.lower() != "readme.md":
            improve_code(file_path)

    print("📝 Generating README.md ...")
    files_summary = summarize_repo([str(f.relative_to(repo_root)) for f in editable_files])
    readme_content = generate_readme(repo_name, files_summary)

    if readme_content:
        Path("README.md").write_text(readme_content, encoding="utf-8")
        print("✅ README.md updated")

    print("\n🎉 Repository improved successfully!")
    print("👉 Run: git status")


if __name__ == "__main__":
    main()
```