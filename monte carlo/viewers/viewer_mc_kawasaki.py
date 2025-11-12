import matplotlib.pyplot as plt
import numpy as np
import json
from plot_handlers import general_plot,general_save
from cluster_handlers import get_clusters,get_cluster_number
from collections import Counter

def general_cluster_plot(steplist,clusterlists,title):
	for inf in clusterlists.keys():
		general_plot(steplist[1:],clusterlists[inf][1:],["pasos","número de grupos"],title)
	plt.legend([f"≥{elem}" for elem in clusterlists.keys()])
	#general_save(f"mode=c {title}")
	#plt.show()

def general_coverTi_plot(Alist,ilist,xlabel,title):
	infs = ilist[0].keys()
	for inf in infs:
		ydata = []
		for index in range(len(Alist)):
			ydata.append(ilist[index][inf])
		general_plot(Alist,ydata,[xlabel,"número de grupos promedio"],title)
	plt.legend([f"≥{inf}" for inf in infs])
	#general_save(f"mode=a {title}")
	#plt.show()

def general_matrix_plot(matrix,clusters,title):
	tmpmatrix = matrix
	for kcluster in clusters.keys():
		for indexes in clusters[kcluster]:
			(i,j) = indexes
			tmpmatrix[i][j] = kcluster+1000
	plt.matshow(tmpmatrix, cmap="rainbow")
	plt.title(title)
	#general_save(f"mode=m {title}")
	#plt.show()

def histogram_cluster_plot(clusters,title,inf=0):
	plt.xlabel("tamaño de grupos")
	plt.ylabel("número de grupos") # (log)
	rlow,rhigh,rstep = [2,7,1]
	fig, ax = plt.subplots()
	#ax.set_yscale("log")
	#ax.set_xscale("log")
	for inft in range(rlow,rhigh,rstep):
		ckeys = clusters.keys()
		tmp = [len(clusters[key]) for key in ckeys if len(clusters[key]) >= inft]
		data = Counter(tmp)
		xdata = list(data.keys())
		xdata.sort()
		ydata = [data[key] for key in xdata]
		print(data)
		plt.plot(xdata, ydata, '-o')
	plt.legend([f"≥{inf}" for inf in range(rlow,rhigh,rstep)])
	plt.title(f"{title} (≥{rlow}-{rhigh})")
	#general_save(f"mode=h {title} (≥{inf})")
	#plt.show()

def process_json(json_name,rdata):
	# opening file
	with open(json_name) as file:
		json_contents = json.load(file)
	print(sum([json_contents[key]["time"] for key in json_contents.keys()]))
	res = {}
	Clist = {}
	Tlist = {}
	# iterating over the dictionary
	for key in json_contents.keys():
		res[key] = {}
		# key variables to consider
		listkey = key.replace("(", "").replace(")", "").split(",")
		[cover,Ti] = list(map(float, listkey))
		n = json_contents[key]["n"]
		laststep = json_contents[key]["laststep"]
		selections = json_contents[key]["selections"]
		steplist = []
		rlow,rhigh,rstep = rdata
		clusterlists = {inf:[] for inf in range(rlow,rhigh,rstep)}
		islandmean = {inf:0 for inf in range(rlow,rhigh,rstep)}
		# iterating over the steps
		for step in selections.keys():
			# getting the clusters
			matrix = selections[step]["matrix"]
			clusters = get_clusters(matrix,n,0)
			steplist.append(int(step))
			for inf in clusterlists.keys():
				clusterlists[inf].append(get_cluster_number(clusters,inf=inf))
		for inf in clusterlists.keys():
			islandmean[inf] = np.mean(clusterlists[inf][-laststep:])
		res[key]["steplist"] = steplist
		res[key]["clusterlists"] = clusterlists
		if cover not in Clist.keys():
			Clist[cover] = {}
		Clist[cover][Ti] = islandmean
		if Ti not in Tlist.keys():
			Tlist[Ti] = {}
		Tlist[Ti][cover] = islandmean
	#print(Clist)
	#print(Tlist)
	return res,Clist,Tlist

def time_kawasaki(json_name,folder="test"):
	# opening file
	with open(json_name) as file:
		json_contents = json.load(file)
	data = {eval(key):json_contents[key]["time"] for key in json_contents.keys()}
	qs = list(set([key[0] for key in data.keys()]))
	qs.sort()
	Ts = list(set([key[1] for key in data.keys()]))
	Ts.sort()
	print(Ts)
	for q in qs:
		title = "cubrimientos vs tiempo de ejecución"
		qTs = [key[1] for key in data.keys() if key[0] == q]
		times = [data[key] for key in data.keys() if key[0] == q]
		general_plot(qTs,times,["T","tiempo (s)"],title)
	plt.legend([f"{q}" for q in qs])
	filename = "qtimes"
	general_save(f"{folder}/kawasaki/t",filename)
	plt.show()
	for T in Ts:
		title = "temperatura vs tiempo de ejecución"
		Tqs = [key[0] for key in data.keys() if key[1] == T]
		times = [data[key] for key in data.keys() if key[1] == T]
		general_plot(Tqs,times,["cubrimientos","tiempo (s)"],title)
	plt.legend([f"{T} K" for T in Ts])
	filename = "Ttimes"
	general_save(f"{folder}/kawasaki/t",filename)
	plt.show()

def view_kawasaki(json_name,res,Clist,Tlist,folder="test",mode="c"):
	# opening file
	with open(json_name) as file:
		json_contents = json.load(file)
	print(sum([json_contents[key]["time"] for key in json_contents.keys()]))
	for key in json_contents.keys():
		listkey = key.replace("(", "").replace(")", "").split(",")
		[cover,Ti] = list(map(float, listkey))
		n = json_contents[key]["n"]
		nstep = json_contents[key]["nstep"]
		selections = json_contents[key]["selections"]
		steplist = res[key]["steplist"]
		clusterlists = res[key]["clusterlists"]
		for step in selections.keys():
			matrix = selections[step]["matrix"]
			clusters = get_clusters(matrix,n)
			if mode=="f" and int(step)==nstep-1:
				title = f"n={n} T={Ti} K cubrim.={cover} paso={int(step):04d}"
				histogram_cluster_plot(clusters,title,inf=2)
				filename = f"n{n}T{Ti}c{cover}s{int(step):04d}h"
				general_save(f"{folder}/kawasaki/{mode}",filename)
				plt.show()
				"""
				filename = f"n{n}T{Ti}c{cover}s{int(step):04d}m"
				general_matrix_plot(matrix,clusters,title)
				general_save(f"{folder}/kawasaki/{mode}",filename)
				plt.show()
				"""
			if mode=="m":
				title = f"n={n} cubrim.={cover} T={Ti} K paso={int(step):04d}"
				general_matrix_plot(matrix,clusters,title)
				filename = f"n{n}T{Ti}c{cover}s{int(step):04d}"
				general_save(f"{folder}/kawasaki/{mode}",filename)
				plt.show()
		if mode=="c":
			title = f"n={n} T={Ti} K cubrimiento={cover}"
			general_cluster_plot(steplist,clusterlists,title)
			filename = f"n{n}T{Ti}c{cover}"
			general_save(f"{folder}/kawasaki/{mode}",filename)
			plt.show()
	if mode=="a":
		for C in Clist.keys():
			Ts = Clist[C].keys()
			ilist = list(Clist[C].values())
			title = f"n={n} cubrimiento={C}"
			general_coverTi_plot(Ts,ilist,"T (K)",title)
			filename = f"n{n}c{C}"
			general_save(f"{folder}/kawasaki/{mode}",filename)
			plt.show()
		for Ti in Tlist.keys():
			Cs = Tlist[Ti].keys()
			ilist = list(Tlist[Ti].values())
			title = f"n={n} T={Ti} K"
			general_coverTi_plot(Cs,ilist,"cubrimiento",title)
			filename = f"n{n}T{Ti}"
			general_save(f"{folder}/kawasaki/{mode}",filename)
			plt.show()
