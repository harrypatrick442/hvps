#!/bin/bash
set -e

# === USER CONFIGURATION ===
B64_FILE="D:/test/dump.b64"                   # Input Base64 file from ESP32
CORE_ELF_FILE="D:/temp/core_dump.elf"         # Decoded binary core dump output
FIRMWARE_ELF="C:/repos/hvps/Code/C++/HVPSController/build/HVPSController.elf" # Path to your firmware ELF from build/
ESP_CORE_DUMP_TOOL="C:/Users/USER1/esp/v5.4/esp-idf/components/espcoredump/espcoredump.py"
# ===========================

# Check dependencies
command -v python >/dev/null 2>&1 || { echo "[!] Python not found"; exit 1; }
[ -f "$B64_FILE" ] || { echo "[!] Base64 dump file not found: $B64_FILE"; exit 1; }
[ -f "$FIRMWARE_ELF" ] || { echo "[!] Firmware ELF not found: $FIRMWARE_ELF"; exit 1; }
[ -f "$ESP_CORE_DUMP_TOOL" ] || { echo "[!] espcoredump.py not found: $ESP_CORE_DUMP_TOOL"; exit 1; }

echo "[*] Decoding Base64 dump -> $CORE_ELF_FILE ..."
python - <<EOF
import base64
data = open("$B64_FILE", "rb").read()
decoded = base64.b64decode(data)
open("$CORE_ELF_FILE", "wb").write(decoded)
print(f"[+] Wrote {len(decoded)} bytes to $CORE_ELF_FILE")
EOF

echo "[*] Running espcoredump.py info_corefile ..."
python "$ESP_CORE_DUMP_TOOL" info_corefile "$CORE_ELF_FILE" "$FIRMWARE_ELF"

echo "[✓] Done! Human-readable crash info printed above."
