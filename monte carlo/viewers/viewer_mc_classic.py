import numpy as np
import matplotlib.pyplot as plt
import json

def view_classic(json_name,dropkeys):
	with open(json_name) as file:
		json_contents = json.load(file)
	print(sum([json_contents[key]["time"] for key in json_contents.keys()]))
	for key in dropkeys:
		del json_contents[key]
	print(json_contents.keys())
	modes = {
		0:"cubrimiento promedio",
		1:"vecinos total promedio",
		2:"primeros vecinos promedio",
		3:"segundos vecinos promedio"
	}
	for mode in modes.keys():
		T = []
		for Ti in json_contents.keys():
			dicchosen = json_contents[Ti]
			mubot,mutop,mun = dicchosen["muinfo"]
			averagesmu = [item[mode] for item in dicchosen["averagesmu"]]
			istart = 0
			iend = len(averagesmu)
			plt.plot(np.linspace(mubot,mutop,mun)[istart:iend],averagesmu[istart:iend],"--o")
			T.append(float(Ti))
			title = f"{modes[mode]} {'(filtrado)' if dropkeys != [] else '(completo)'}"
			plt.title(title)
			plt.xlabel("valores μ")
			plt.ylabel(f"{modes[mode]}")
		plt.legend([f"{Ti} K" for Ti in T])
		plt.savefig(f"../png_files/{title}.png")
		plt.show()

filenames = [
	"../out_files/mc_classic (n=200, muinfo=(-1.000000,-0.250000,200), steps=100).json"
]

dropkeys = ["400.000000","500.000000","700.000000","800.000000","900.000000","1000.000000"]

for filename in filenames:
	view_classic(filename,dropkeys)
	view_classic(filename,[])
