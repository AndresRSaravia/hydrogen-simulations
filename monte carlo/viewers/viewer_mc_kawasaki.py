import matplotlib.pyplot as plt
import numpy as np
import json

def auxf(index,n,d):
	return (index+d)%n

def get_clusters(matrix,n,diagflag=0):
	hydrogens = [(i,j) for i in range(n) for j in range(n) if (matrix[i][j]==1)]
	queued = np.zeros((n,n))
	clusters = {}
	ncluster = 0
	while (hydrogens != []):
		first = hydrogens.pop(0)
		queue = [first]
		i = first[0]
		j = first[1]
		queued[i][j] = 1
		marked = []
		while (queue != []):
			elem = queue.pop(0)
			#print(elem)
			i = elem[0]
			j = elem[1]
			ip1 = auxf(i,n,+1)
			im1 = auxf(i,n,-1)
			jp1 = auxf(j,n,+1)
			jm1 = auxf(j,n,-1)
			if (matrix[ip1][j]==1 and queued[ip1][j]==0):
				queue.append((ip1,j))
				queued[ip1][j] = 1
			if (matrix[im1][j]==1 and queued[im1][j]==0):
				queue.append((im1,j))
				queued[im1][j] = 1
			if (matrix[i][jp1]==1 and queued[i][jp1]==0):
				queue.append((i,jp1))
				queued[i][jp1] = 1
			if (matrix[i][jm1]==1 and queued[i][jm1]==0):
				queue.append((i,jm1))
				queued[i][jm1] = 1
			if (diagflag!=0):
				if (matrix[ip1][jp1]==1 and queued[ip1][jp1]==0):
					queue.append((ip1,jp1))
					queued[ip1][jp1] = 1
				if (matrix[im1][jp1]==1 and queued[im1][jp1]==0):
					queue.append((im1,jp1))
					queued[im1][jp1] = 1
				if (matrix[ip1][jm1]==1 and queued[ip1][jm1]==0):
					queue.append((ip1,jm1))
					queued[ip1][jm1] = 1
				if (matrix[im1][jm1]==1 and queued[im1][jm1]==0):
					queue.append((im1,jm1))
					queued[im1][jm1] = 1
			marked.append(elem)
		clusters[ncluster] = marked
		ncluster += 1
		while (hydrogens !=[] and queued[hydrogens[0][0],hydrogens[0][1]] == 1):
			hydrogens.pop(0)
	return clusters

def get_cluster_number(clusters,inf=0,sup=float("inf")):
	nclusters = 0
	for key in clusters.keys():
		tmp = len(clusters[key])
		if inf <= tmp and tmp <= sup:
			nclusters += 1
	return nclusters

def general_plot(x,y,labels,title):
	xlabel,ylabel = labels
	plt.xlabel(xlabel)
	plt.ylabel(ylabel)
	plt.title(title)
	plt.plot(x,y,marker="o")

def general_save(title):
	plt.savefig(f"../png_files/{title}.png")

def general_cluster_plot(steplist,clusterlists,title):
	for inf in clusterlists.keys():
		general_plot(steplist[1:],clusterlists[inf][1:],["pasos","número de clústeres"],title)
	plt.legend([f"≥{elem}" for elem in clusterlists.keys()])
	#general_save(f"mode=c {title}")
	plt.show()

def general_coverTi_plot(Alist,ilist,xlabel,title):
	infs = ilist[0].keys()
	for inf in infs:
		ydata = []
		for index in range(len(Alist)):
			ydata.append(ilist[index][inf])
		general_plot(Alist,ydata,[xlabel,"número de clústeres promedio"],title)
	plt.legend([f"≥{inf}" for inf in infs])
	#general_save(f"mode=a {title}")
	plt.show()

def general_matrix_plot(matrix,clusters,title):
	tmpmatrix = matrix
	for kcluster in clusters.keys():
		for indexes in clusters[kcluster]:
			(i,j) = indexes
			tmpmatrix[i][j] = kcluster+1000
	plt.matshow(tmpmatrix, cmap="rainbow")
	plt.title(title)
	#general_save(f"mode=m {title}")
	plt.show()

def histogram_cluster_plot(clusters,title,inf=0):
	plt.xlabel("tamaño de clústeres")
	plt.ylabel("número de clústeres")
	rlow,rhigh,rstep = [2,7,1]
	for inft in range(rlow,rhigh,rstep):
		plt.hist([len(clusters[key]) for key in clusters.keys() if len(clusters[key]) >= inft], bins=25)
	plt.title(f"{title} (≥{rlow}-{rhigh})")
	#general_save(f"mode=h {title} (≥{inf})")
	plt.show()

def process_json(json_name,rdata=[1,6,1]):
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
	print(Clist)
	print(Tlist)
	return res,Clist,Tlist

def view_kawasaki(json_name,res,Clist,Tlist,mode="c"):
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
			if mode=="h" and int(step)==nstep-1:
				title = f"n={n} T={Ti} K cubrim.={cover} paso={int(step):04d}"
				histogram_cluster_plot(clusters,title,inf=2)
				general_matrix_plot(matrix,clusters,title)
			if mode=="m":
				title = f"n={n} cubrim.={cover} T={Ti} K paso={int(step):04d}"
				general_matrix_plot(matrix,clusters,title)
		if mode=="c":
			title = f"n={n} T={Ti} K cubrimiento={cover}"
			general_cluster_plot(steplist,clusterlists,title)
	if mode=="a":
		for C in Clist.keys():
			Ts = Clist[C].keys()
			ilist = list(Clist[C].values())
			title = f"n={n} cubrimiento={C}"
			general_coverTi_plot(Ts,ilist,"T (K)",title)
		for Ti in Tlist.keys():
			Cs = Tlist[Ti].keys()
			ilist = list(Tlist[Ti].values())
			title = f"n={n} T={Ti} K"
			general_coverTi_plot(Cs,ilist,"cubrimiento",title)

filenames = [
	"../out_files/mc_kawasaki (n=200, steps=3000).json"
]

rdata = [2,7,1] # [1,6,1] [6,9,1] range(100,201,50)
rlow,rhigh,rstep = rdata
for filename in filenames:
	res,Clist,Tlist = process_json(filename,rdata)
	for mode in ["h"]: # "h","m","c","a"
		view_kawasaki(filename,res,Clist,Tlist,mode=mode)
