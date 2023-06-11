import os

# Destination directory
dest_dir = 'test'

# Get the file list from the configuration file
with open('test/test_files.cfg', 'r') as f:
    files_to_delete = [line.strip() for line in f]

# Delete each file
for file in files_to_delete:
    try:
        os.remove(os.path.join(dest_dir, file))
    except FileNotFoundError:
        pass  # File already deleted or not copied in the first place
