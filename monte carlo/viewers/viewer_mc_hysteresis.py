import numpy as np
import matplotlib.pyplot as plt
import json

def view_hysteresis(json_name):
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
			averagesmu1 = averagesmu[:int(len(averagesmu)/2)]
			averagesmu2 = averagesmu[int(len(averagesmu)/2):]
			averagesmu2.reverse()
			istart = 0
			iend = len(averagesmu1)
			plt.plot(np.linspace(mubot,mutop,mun)[istart:iend],averagesmu1[istart:iend],'--o')
			plt.plot(np.linspace(mubot,mutop,mun)[istart:iend],averagesmu2[istart:iend],'--o')
			T.append(int(dicchosen["Ti"]))
			plt.legend(["ida","vuelta"])
			plt.title(modes[mode]+" T: "+str(dicchosen["Ti"]))
			plt.xlabel('valores μ')
			plt.ylabel('promedios')
			plt.show()

view_hysteresis('../out_files/mc_hysteresis.json')