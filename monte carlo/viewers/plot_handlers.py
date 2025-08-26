import matplotlib.pyplot as plt

def general_plot(x,y,labels,title=""):
	xlabel,ylabel = labels
	plt.xlabel(xlabel)
	plt.ylabel(ylabel)
	plt.title(title)
	plt.plot(x,y,marker="o")

def general_save(folder,filename):
	plt.savefig(f"../png_files/{folder}/{filename}.png")
