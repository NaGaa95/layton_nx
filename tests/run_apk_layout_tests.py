import os
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
source = (root / "source/main.c").read_text(encoding="utf-8")
start = source.index("#define TURNSTILE_A_OFF")
end = source.index("// patch an exported engine function", start)
with tempfile.TemporaryDirectory(ignore_cleanup_errors=True) as temporary:
    build = Path(temporary)
    (build / "apk_layout.inc").write_text(source[start:end], encoding="utf-8")
    exe = build / ("apk_layout_test.exe" if os.name == "nt" else "apk_layout_test")
    subprocess.run([os.environ.get("CC", "gcc"), "-std=c11", "-Wall", "-Wextra", "-Werror",
                    "-I" + str(build), str(root / "tests/apk_layout_test.c"), "-o", str(exe)],
                   check=True)
    subprocess.run([str(exe)], check=True)
