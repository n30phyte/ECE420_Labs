from __future__ import print_function

import os
import subprocess

subprocess.call(["make", "clean"])
subprocess.call(["make"])

threads = ["1", "2", "4", "5", "10", "20"]
sizes = ["5300", "13000", "18789"]

with open("hosts") as hosts_file:
    lines = hosts_file.readlines()
    lines.pop(0)

    for size in sizes:
        subprocess.call(["./datatrim", "-b", size])

        for host in lines:
            subprocess.call("scp * %s:~" % host.strip(), shell=True)
        
        for i in range(10):
            print("=========================")
            print("Run number: " + str(i))
            print("=========================")
            for thread in threads:
                subprocess.call(
                    [
                        "mpirun",
                        "-np",
                        thread,
                        "-f", "hosts",
                        "./main",
                    ]
                )
                result = subprocess.call(["./serialtester"])

                if result != 0:
                    print("Error for thread count: " + thread + " size: " + size)
                    break

                os.rename("./data_output", "./data_output_" + thread + "_" + size + "_run_" + str(i))
