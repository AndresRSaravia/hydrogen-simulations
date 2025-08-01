import numpy as np
import matplotlib.pyplot as plt
import json

def view_hysteresis(json_name):
	with open(json_name) as file:
		json_contents = json.load(file)
	print(sum([json_contents[key]["time"] for key in json_contents.keys()]))
	print(json_contents.keys())
	modes = {
		0:"θ mean",
		#1:"Vecinos total promedio",
		#2:"Primeros vecinos promedio",
		#3:"Segundos vecinos promedio"
	}
	for mode in modes.keys():
		T = []
		for Ti in json_contents.keys():
			dicchosen = json_contents[Ti]
			mubot,mutop,mun = dicchosen["muinfo"]
			averagesmu = [item[mode] for item in dicchosen["averagesmu"]]
			averagesmu1 = averagesmu[:int(len(averagesmu)/2)]
			averagesmu2 = averagesmu[int(len(averagesmu)/2):]
			averagesmu2.reverse()
			istart = 0
			iend = len(averagesmu1)
			plt.plot(np.linspace(mubot,mutop,mun)[istart:iend],averagesmu1[istart:iend],"--o")
			plt.plot(np.linspace(mubot,mutop,mun)[istart:iend],averagesmu2[istart:iend],"--o")
			T.append(float(Ti))
			plt.legend(["(->)","(<-)"])
			title = f"{modes[mode]} T={str(float(Ti)}"
			plt.title(title)
			plt.xlabel("μ values")
			plt.ylabel("θ means")
			plt.savefig(f"../png_files/{title}.png")
			plt.show()

filenames = [
	"../out_files/mc_hysteresis (n=200, muinfo=(-0.690000,-0.655000,400), steps=100).json"
]

for filename in filenames:
	view_hysteresis(filename)