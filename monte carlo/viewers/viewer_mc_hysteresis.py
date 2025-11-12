import matplotlib.pyplot as plt
import numpy as np
import json
from folder_handlers import create_folder
from plot_handlers import general_plot,general_save

def time_hysteresis(json_name,folder="test"):
	# opening file
	with open(json_name) as file:
		json_contents = json.load(file)
	for key in json_contents.keys():
		print(key,json_contents[key]["time"])
	data = {eval(key):json_contents[key]["time"] for key in json_contents.keys()}
	title = "temperatura vs tiempo de ejecución"
	Ts = data.keys()
	times = data.values()
	general_plot(Ts,times,["T (K)","tiempo (s)"],title,marker="o")
	#plt.legend([f"{T} K" for T in Ts])
	filename = "Ttimes"
	general_save(f"{folder}/hysteresis",filename)
	plt.show()

def view_hysteresis(json_name,folder="test"):
	with open(json_name) as file:
		json_contents = json.load(file)
	print(sum([json_contents[key]["time"] for key in json_contents.keys()]))
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
			averagesmu1 = averagesmu[:int(len(averagesmu)/2)]
			averagesmu2 = averagesmu[int(len(averagesmu)/2):]
			averagesmu2.reverse()
			istart = 0
			iend = len(averagesmu1)
			x = np.linspace(mubot,mutop,mun)[istart:iend]
			y1 = averagesmu1[istart:iend]
			y2 = averagesmu2[istart:iend]
			labels = ["valores μ",f"{modes[mode]}"]
			title = f"{modes[mode]} T={float(Ti)} K"
			general_plot(x,y1,labels,title)
			general_plot(x,y2,labels,title)
			T.append(float(Ti))
			plt.legend(["ida (->)","vuelta (<-)"])
			filename = f"n{n}m{mode}T{float(Ti)}"
			general_save(f"{folder}/hysteresis",filename)
			plt.show()
