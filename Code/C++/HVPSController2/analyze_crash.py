import os
import sys
import base64
import subprocess
from pathlib import Path

# === USER CONFIGURATION ===
B64_FILE = Path(r"D:\test\dump.b64")   # Input Base64 crash dump
CORE_ELF = Path(r"D:\temp\core_dump.elf")  # Output ELF file (decoded dump)
FIRMWARE_ELF = Path(r"C:\repos\hvps\Code\C++\HVPSController\build\HVPSController.elf")
ESP_COREDUMP_TOOL = Path(r"C:\Users\USER1\esp\v5.4\esp-idf\components\espcoredump\espcoredump.py")
SAVE_REPORT_AS = Path(r"D:\temp\crash_report.txt")  # Optional; set to None to disable file saving
# ===========================


def check_file(path: Path, description: str):
    """Ensure a required file exists."""
    if not path.exists():
        print(f"❌ {description} not found:\n   {path}")
        sys.exit(1)


def decode_base64_to_file(src: Path, dst: Path):
    """Decode Base64 input file into binary ELF file."""
    print(f"🔍 Decoding Base64 dump → {dst}")
    try:
        data = src.read_bytes()
        decoded = base64.b64decode(data)
        dst.write_bytes(decoded)
        print(f"✅ Wrote {len(decoded)} bytes to {dst}")
    except Exception as e:
        print(f"❌ Failed to decode Base64: {e}")
        sys.exit(1)
        
def run_espcoredump(core_elf: Path, firmware_elf: Path, espcoredump_tool: Path) -> str:
    """Run modern espcoredump.py (ESP-IDF ≥ 5.4) to analyze the crash dump."""
    print("\n🧩 Running espcoredump analyzer...\n──────────────────────────────")

    cmd = [
        sys.executable,
        str(espcoredump_tool),
        "info_corefile",
        "--core", str(core_elf),
        "--core-format", "raw",
        "--gdb", "C:/Users/USER1/esp/xtensa-esp32-elf/bin/xtensa-esp32-elf-gdb.exe",
        str(firmware_elf)
    ]

    print(f"[*] Executing: {' '.join(cmd)}\n")
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=False)
    except FileNotFoundError:
        print("❌ Could not find Python or espcoredump.py!")
        sys.exit(1)

    output = result.stdout + "\n" + result.stderr
    if result.returncode != 0:
        print("⚠️ espcoredump failed — check output below.")
    print(output)
    return output



def main():
    print("\n🧠 ESP32 Crash Analyzer\n──────────────────────────────\n")

    # Step 1: Verify dependencies
    check_file(B64_FILE, "Base64 dump file")
    check_file(FIRMWARE_ELF, "Firmware ELF")
    check_file(ESP_COREDUMP_TOOL, "espcoredump.py")

    # Step 2: Decode the Base64 dump
    decode_base64_to_file(B64_FILE, CORE_ELF)

    # Step 3: Run espcoredump
    output = run_espcoredump(CORE_ELF, FIRMWARE_ELF, ESP_COREDUMP_TOOL)

    # Step 4: Save output
    if SAVE_REPORT_AS:
        try:
            SAVE_REPORT_AS.write_text(output, encoding="utf-8")
            print(f"\n📝 Saved crash report to: {SAVE_REPORT_AS}")
        except Exception as e:
            print(f"⚠️ Failed to save crash report: {e}")

    print("\n✅ Done! Human-readable crash info printed above.\n")


if __name__ == "__main__":
    main()
