import matplotlib.pyplot as plt
import sys
import os


if len(sys.argv) == 1:
	print("Filename expected!")
	sys.exit(1)

if not os.path.isfile(sys.argv[1]):
	print("File does not exist!")
	sys.exit(1)

with open(sys.argv[1], "r") as file:
	content = file.readlines()

content = [int(k) for k in content]

plt.plot(content);
plt.show()
