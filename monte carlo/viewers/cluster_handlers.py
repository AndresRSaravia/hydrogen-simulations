import numpy as np

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
