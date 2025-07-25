import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import ListedColormap
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

def get_cluster_number(clusters,inf=0,sup=float('inf')):
	nclusters = 0
	for key in clusters.keys():
		tmp = len(clusters[key])
		if inf < tmp and tmp < sup:
			nclusters += 1
	return nclusters

def general_plot(x,y,title):
	plt.plot(x,y,marker = 'o')

def general_matrix_plot(matrix,clusters,title):
	tmpmatrix = matrix
	for kcluster in clusters.keys():
		for indexes in clusters[kcluster]:
			(i,j) = indexes
			tmpmatrix[i][j] = kcluster+100
	plt.matshow(tmpmatrix, cmap='rainbow')
	plt.title(title)
	plt.show()

def histogram_cluster_plot(clusters,title,inf=0):
	plt.hist([len(clusters[key]) for key in clusters.keys() if len(clusters[key]) > inf])
	plt.title(title)
	plt.show()

def view_kawasaki(json_name):
	with open(json_name) as file:
		json_contents = json.load(file)
	print(sum([json_contents[key]['time'] for key in json_contents.keys()]))
	print(json_contents.keys())
	for key in json_contents.keys():
		n = json_contents[key]["n"]
		Ti = json_contents[key]["Ti"]
		niter = json_contents[key]["niter"]
		selections = json_contents[key]['selections']
		iterlist = []
		clusterlists = {elem:[] for elem in range(5,8,1)} #range(100,201,50)
		for iteration in selections.keys():
			selected = selections[iteration]
			if ("matrix" in selected):
				matrix = selected["matrix"]
				cover = np.mean(matrix)
				clusters = get_clusters(matrix,n,0)
				title = f"n={n} T={Ti} cover={cover} iter={int(iteration):04d}/{niter-1}"
				histogram_cluster_plot(clusters,title,inf=20)
				#general_matrix_plot(matrix,clusters,title)
				iterlist.append(int(iteration))
				for inf in clusterlists.keys():
					clusterlists[inf].append(get_cluster_number(clusters,inf=inf))
				print(Ti,iteration,get_cluster_number(clusters),get_cluster_number(clusters,inf=1),get_cluster_number(clusters,inf=2))
		if False:
			title = f"n={n} T={Ti} cover={cover}"
			plt.title(title)
			for inf in clusterlists.keys():
				print(1)
				general_plot(iterlist[1:],clusterlists[inf][1:],title)
			plt.legend([f">{elem}" for elem in clusterlists.keys()])
			plt.show()

filenames = [
	"../out_files/mc_kawasaki (200x200, 0.3, 3000 iter).json",
	"../out_files/mc_kawasaki (200x200, 0.5, 3000 iter).json",
	"../out_files/mc_kawasaki (200x200, 0.7, 3000 iter).json"
]

for filename in filenames:
	view_kawasaki(filename)
