# Converts all .png files in images/ to indexed 256-color .bin files in data/
# Format: 512-byte palette (256 × 2B big-endian RGB565) + 57600 pixel indices
# usage: python convert.py
from PIL import Image
from pathlib import Path
import struct

src = Path("images")
dst = Path("data")
dst.mkdir(exist_ok=True)

for png in sorted(src.glob("*.png")):
    out = dst / png.with_suffix(".bin").name
    rgba = Image.open(png).convert("RGBA")
    bg = Image.new("RGB", rgba.size, (255, 255, 255))
    bg.paste(rgba, mask=rgba.split()[3])
    img = bg.resize((240, 240), Image.LANCZOS)
    img_q = img.quantize(colors=256, method=Image.Quantize.MEDIANCUT)
    palette_rgb = img_q.getpalette()[:768]

    with open(out, "wb") as f:
        # palette: 256 entries × 2 bytes big-endian RGB565
        for i in range(256):
            r, g, b = palette_rgb[i * 3], palette_rgb[i * 3 + 1], palette_rgb[i * 3 + 2]
            color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            f.write(struct.pack(">H", color))
        # pixel indices (1 byte each)
        f.write(bytes(img_q.getdata()))

    print(f"{png} -> {out} ({out.stat().st_size} bytes)")
