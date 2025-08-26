import matplotlib.pyplot as plt
import numpy as np
import json
from folder_handlers import create_folder
from plot_handlers import general_plot,general_save

def time_classic(json_name,folder="test"):
	# opening file
	with open(json_name) as file:
		json_contents = json.load(file)
	for key in json_contents.keys():
		print(key,json_contents[key]["time"])
	data = {eval(key):json_contents[key]["time"] for key in json_contents.keys()}
	title = "temperatura vs tiempo de ejecución"
	Ts = data.keys()
	times = data.values()
	general_plot(Ts,times,["T (K)","tiempo (s)"],title)
	#plt.legend([f"{T} K" for T in Ts])
	filename = "Ttimes"
	general_save(f"{folder}/classic",filename)
	plt.show()

def view_classic(json_name,dropkeys,code,folder="test"):
	with open(json_name) as file:
		json_contents = json.load(file)
	print(sum([json_contents[key]["time"] for key in json_contents.keys()]))
	for key in dropkeys:
		del json_contents[key]
	print(json_contents.keys())
	modes = {
		0:"cubrimiento promedio",
		#1:"vecinos total promedio",
		#2:"primeros vecinos promedio",
		#3:"segundos vecinos promedio"
	}
	for mode in modes.keys():
		T = []
		for Ti in json_contents.keys():
			n = json_contents[Ti]["n"]
			dicchosen = json_contents[Ti]
			mubot,mutop,mun = dicchosen["muinfo"]
			averagesmu = [item[mode] for item in dicchosen["averagesmu"]]
			istart = 0
			iend = len(averagesmu)
			labels = ["valores μ",f"{modes[mode]}"]
			if code!="c":
				title = f"{modes[mode]} {'(filtrado)' if dropkeys != [] else '(completo)'}"
			else:
				title = f"{modes[mode]} T={float(Ti)}"
			x = np.linspace(mubot,mutop,mun)[istart:iend]
			y = averagesmu[istart:iend]
			general_plot(x,y,labels,title)
			T.append(float(Ti))
			if code=="c":
				filename = f"n{n}m{mode}c{code}T{float(Ti)}"
				general_save(f"{folder}/classic",filename)
				plt.show()
		if code!="c":
			plt.legend([f"{Ti} K" for Ti in T])
			filename = f"n{n}m{mode}c{code}"
			general_save(f"{folder}/classic",filename)
			plt.show()
