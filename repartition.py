import matplotlib.pyplot as plt
import sys
import os


if len(sys.argv) == 1:
	print("At least one filename expected!")
	sys.exit(1)


for filename in sys.argv[1:]:

	print("opening ", filename)

	if not os.path.isfile(filename):
		print("File does not exist!")
	else:

		content = []
		with open(filename, "r") as file:
			content = file.readlines()
	
		content = [int(k) for k in content]
	
		total = sum(content)
	
		print("Average count hit = ", total/len(content))
	
		plt.figure(num=filename)
		plt.plot(content);
		alt_name = filename.replace("txt", "png")
		plt.savefig(alt_name)


#plt.show()
