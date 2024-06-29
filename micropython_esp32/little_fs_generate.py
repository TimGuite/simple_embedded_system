
# Source (MIT): https://github.com/georgik/esp32-lang-lab/blob/ac183c04f68bd619ce31ffbfd1caae38b60dc21e/support/python/littlefs_generate.py
# Source: https://github.com/minasouliman/wokwi_esp32_micropython/blob/main/tools/filesystem_generate.py
 
import os
from littlefs import LittleFS
import subprocess

output_image = "build/littlefs.img"

lfs = LittleFS(block_size=4096, block_count=512, prog_size=256)

for filename in ["main.py", "driver_dht20.py"]:
    with open(filename, 'rb') as src_file:
        with  lfs.open(filename, 'wb') as lfs_file:
            lfs_file.write(src_file.read())

with open(output_image, 'wb') as fh:
    fh.write(lfs.context.buffer)


#subprocess.run(f"esptool.py --chip esp32 merge_bin -o {workspace_dir}/out.bin --flash_mode dio --flash_size 4MB 0x1000 {workspace_dir}/ESP32_GENERIC-20231227-v1.22.0.bin 0x200000 {workspace_dir}/littlefs.img", shell=True)