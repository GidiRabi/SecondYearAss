import subprocess
import filecmp
import os

# Define the names of the student's and correct output files
student_output_file = "my_output.txt"
correct_output_file = "output.txt"


# Get the current working directory
current_directory = os.getcwd()

# Change to the directory containing main.py
os.chdir(current_directory)

# Run the main.py file and save its output to student_output_file
try:
    subprocess.run(
        ["python3", "../main.py"],
        stdout=open(student_output_file, "w+"),
        stderr=subprocess.PIPE,
        check=True,
        cwd=current_directory,
    )
except subprocess.CalledProcessError as e:
    print(e)
    print(f"Error running main.py: {e.stderr.decode('utf-8')}")
    exit(1)

# Compare the student's output to the correct output
if filecmp.cmp(student_output_file, correct_output_file, shallow=False):
    print("Output is correct. You passed the test successfully!")
else:
    print("Output is incorrect. You are required to fix your code")
