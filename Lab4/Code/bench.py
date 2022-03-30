from __future__ import print_function

import os
import subprocess

subprocess.call(["make", "clean"])
subprocess.call(["make"])

sizes = ["5300", "13000", "18789"]

for size in sizes:
    subprocess.call(["./datatrim", "-b", size])
    subprocess.call(["mpirun",
    "-np", "4",
    "-f", "hosts"
    "./main"])
    result = subprocess.call(["./serialtester"])
    
    if result != 0:
        print("Error for size: " + size)
        break

    os.rename("./data_output", "./data_output_" + size)
