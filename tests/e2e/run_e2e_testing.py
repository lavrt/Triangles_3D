import os
import subprocess
import glob

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

TEST_DATA_DIR = os.path.join(SCRIPT_DIR, "test_data")
PROGRAM_PATH = os.path.join(SCRIPT_DIR, "..", "..", "build", "tests", "run_e2e_test_execution")

if not os.path.exists(TEST_DATA_DIR):
    print(f"Error: directory {TEST_DATA_DIR} was not found")
    exit(1)

if not os.path.exists(PROGRAM_PATH):
    print(f"Error: program {PROGRAM_PATH} was not found")
    exit(1)

for dat_file in glob.glob(os.path.join(TEST_DATA_DIR, "*.dat")):
    output_file = dat_file.replace(".dat", ".ans")
    command = [PROGRAM_PATH, "--data", os.path.basename(dat_file), "--answers", os.path.basename(output_file)]
    print(f"Processing: {os.path.basename(dat_file)}")
    subprocess.run(command)

print("Processing is completed")
