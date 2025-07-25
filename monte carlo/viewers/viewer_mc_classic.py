import numpy as np
import matplotlib.pyplot as plt
import json

def view_classic(json_name):
	with open(json_name) as file:
		json_contents = json.load(file)
	print(sum([json_contents[key]['time'] for key in json_contents.keys()]))
	print(json_contents.keys())
	modes = {
		0:"θ promedio",
		1:"Vecinos total promedio",
		2:"Primeros vecinos promedio",
		3:"Segundos vecinos promedio"
	}
	for mode in modes.keys():
		T = []
		for key in json_contents.keys():
			dicchosen = json_contents[key]
			mubot,mutop,mun = dicchosen['muinfo']
			averagesmu = [item[mode] for item in dicchosen['averagesmu']]
			istart = 0
			iend = len(averagesmu)
			plt.plot(np.linspace(mubot,mutop,mun)[istart:iend],averagesmu[istart:iend],'--o')
			T.append(int(dicchosen["Ti"]))
			plt.legend(["promedio"])
			plt.title(modes[mode]+" T: "+str(dicchosen["Ti"]))
			plt.xlabel('valores μ')
			plt.ylabel('promedios')
			plt.show()

filenames = [
	"../out_files/mc_classic.json",
	"../out_files/mc_classic (backup).json"
]

for filename in filenames:
	view_classic(filename)
