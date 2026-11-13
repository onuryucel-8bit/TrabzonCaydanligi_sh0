#.vs
#out
# dosyalarini yok etmeye yarayan program (teneke kutusuna yazdirdim)
# 12.11.2026

import os
import glob
import shutil

root = "."
folders = [".vs", "out"]


def get_folder_size(path):
    total = 0

    for dirpath, dirnames, filenames in os.walk(path):
        for filename in filenames:
            filepath = os.path.join(dirpath, filename)

            try:
                total += os.path.getsize(filepath)
            except OSError:
                pass

    return total


def format_size(size):
    if size >= 1024 ** 3:
        return f"{size / (1024 ** 3):.2f} GB"
    elif size >= 1024 ** 2:
        return f"{size / (1024 ** 2):.2f} MB"
    elif size >= 1024:
        return f"{size / 1024:.2f} KB"
    else:
        return f"{size} Bytes"


total_deleted = 0

for folder in folders:
    pattern = os.path.join(root, "**", folder)

    for path in glob.glob(pattern, recursive=True):
        if os.path.isdir(path):
            size = get_folder_size(path)

            print(f"Deleting: {path} ({format_size(size)})")

            shutil.rmtree(path)
            total_deleted += size

print("--------------------------------")
print(f"Total deleted: {format_size(total_deleted)}")
print("Cleanup complete!")