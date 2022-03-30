from __future__ import print_function

import os
import subprocess

subprocess.call(["make", "clean"])
subprocess.call(["make"])

threads = ["1", "2", "4", "5", "10", "20"]
sizes = ["5300", "13000", "18789"]

for size in sizes:
    subprocess.call(["./datatrim", "-b", size])

    for thread in threads:
        subprocess.call(
            [
                "mpirun",
                "-np",
                "4",
                # "-f", "hosts"
                "./main",
            ]
        )
        result = subprocess.call(["./serialtester"])

        if result != 0:
            print("Error for thread count: " + thread + " size: " + size)
            break

        os.rename("./data_output", "./data_output_" + thread + "_" + size)
