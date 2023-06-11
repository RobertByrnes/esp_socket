import shutil, os

# Source and destination directories
src_dir = 'src'
dest_dir = 'test'

# Get the file list from the configuration file
with open('test/test_files.cfg', 'r') as f:
    files_to_copy = [line.strip() for line in f]

# Copy each file
for file in files_to_copy:
    shutil.copy(os.path.join(src_dir, file), dest_dir)
