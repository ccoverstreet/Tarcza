# Tarcza: Attenuation packing script
# Cale Overstreet
# Oct 19, 2021

# Used for generating header file used for Tarcza attenuation calculations

import os
import numpy as np
import matplotlib.pyplot as plt

def getPLFileNames():
    raw_files = os.listdir("data")
    pl_files = []

    for file in raw_files:
        if file.endswith(".pl"):
            pl_files.append(file)

    return pl_files

def generateHeaderString(files):
    output = ""

    for i in range(len(files)):
        split_name = files[i][:-3].split("_")
        elem = split_name[0]
        density = split_name[1]
        data = np.genfromtxt("data/"+files[i], skip_header=3) 

        output += "\t{\"" + elem + "\", "
        output += "{\n"
        
        for j in range(len(data)):
            dat = data[j]
            output += "\t\t\t{"
            output += "{}, {}, {}".format(dat[0], dat[1], dat[2])
            output += "}"
            if j != len(data) - 1:
                output += ","

            output += "\n"

        output += "\t\t}"
            

        output += "\n\t}"

        if i != len(files) - 1:
            output += ","

        output += "\n"


    return output
    
def writeHeaderFile(data):
    with open("./coefficients_template.h") as f:
        template = f.read()

        with open("coefficients.h", "w") as f:
            f.write(template.replace("ATTEN_DATA", data))

def main():
    print("Tarcza packing script")

    header_string = generateHeaderString(getPLFileNames())
    writeHeaderFile(header_string)


if __name__ == "__main__":
    main()
