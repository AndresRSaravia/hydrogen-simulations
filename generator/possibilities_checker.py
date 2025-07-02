import numpy as np

poss_pos = {
	0:[
		np.array([[0,0,0],[0,0,0],[0,0,0]])
	],
	1:[
		np.array([[1,0,0],[0,0,0],[0,0,0]]),
		np.array([[0,1,0],[0,0,0],[0,0,0]])
	],
	2:[
		np.array([[1,0,1],[0,0,0],[0,0,0]]),
		np.array([[1,0,0],[0,0,0],[0,0,1]]),
		np.array([[0,1,0],[0,0,0],[0,1,0]]),
		np.array([[0,1,0],[0,0,1],[0,0,0]]),
		np.array([[1,1,0],[0,0,0],[0,0,0]]),
		np.array([[1,0,0],[0,0,1],[0,0,0]])
	],
	3:[
		np.array([[1,0,1],[0,0,0],[0,0,1]]),
		np.array([[0,1,0],[1,0,1],[0,0,0]]),
		np.array([[1,1,1],[0,0,0],[0,0,0]]),
		np.array([[1,0,1],[0,0,1],[0,0,0]]),
		np.array([[1,0,1],[0,0,0],[0,1,0]]),
		np.array([[1,1,0],[0,0,0],[0,0,1]]),
		np.array([[1,1,0],[0,0,0],[0,1,0]]),
		np.array([[1,1,0],[1,0,0],[0,0,0]]),
		np.array([[1,1,0],[0,0,1],[0,0,0]]),
		np.array([[1,0,0],[0,0,1],[0,1,0]])
	],
	4:[
		np.array([[0,1,0],[1,0,1],[0,1,0]]),
		np.array([[1,0,1],[0,0,0],[1,0,1]]),
		np.array([[1,1,1],[0,0,0],[1,0,0]]),
		np.array([[1,0,1],[0,0,1],[1,0,0]]),
		np.array([[1,1,0],[1,0,1],[0,0,0]]),
		np.array([[0,1,0],[1,0,1],[1,0,0]]),
		np.array([[1,1,1],[1,0,0],[0,0,0]]),
		np.array([[1,0,1],[1,0,1],[0,0,0]]),
		np.array([[1,1,1],[0,0,0],[0,1,0]]),
		np.array([[1,0,1],[1,0,0],[0,1,0]]),
		np.array([[1,1,0],[0,0,1],[0,0,1]]),
		np.array([[1,1,0],[0,0,0],[0,1,1]]),
		np.array([[1,1,0],[1,0,0],[0,0,1]])
	],
	5:[
		np.array([[1,1,1],[0,0,0],[1,0,1]]),
		np.array([[1,1,0],[1,0,1],[0,1,0]]),
		np.array([[1,1,1],[1,0,0],[1,0,0]]),
		np.array([[1,1,1],[0,0,1],[1,0,0]]),
		np.array([[1,0,1],[0,0,1],[1,1,0]]),
		np.array([[1,0,1],[1,0,0],[1,1,0]]),
		np.array([[1,1,1],[0,0,0],[1,1,0]]),
		np.array([[1,1,1],[1,0,1],[0,0,0]]),
		np.array([[0,1,0],[1,0,1],[1,0,1]]),
		np.array([[1,1,0],[1,0,1],[1,0,0]]),
		np.array([[1,1,0],[1,0,1],[0,0,1]])
	],
	6:[
		np.array([[1,1,1],[1,0,0],[1,0,1]]),
		np.array([[1,0,1],[1,0,1],[1,0,1]]),
		np.array([[1,1,1],[1,0,1],[0,1,0]]),
		np.array([[1,1,0],[1,0,1],[0,1,1]]),
		np.array([[1,1,1],[1,0,1],[0,0,1]]),
		np.array([[1,1,1],[1,0,0],[0,1,1]])
	],
	7:[
		np.array([[0,1,1],[1,0,1],[1,1,1]]),
		np.array([[1,0,1],[1,0,1],[1,1,1]])
	],
	8:[
		np.array([[1,1,1],[1,0,1],[1,1,1]])
	],
}

number = 0
for i in range(2):
	for j in range(2):
		for k in range(2):
			for m in range(2):
				for n in range(2):
					for o in range(2):
						for p in range(2):
							for q in range(2):
								vari = np.array([[i,j,k],[m,0,o],[o,p,q]])
								count = np.count_nonzero(vari)
								poss_pos_count = poss_pos[count]
								for index in range(len(poss_pos_count)):
									r0 = poss_pos_count[index]
									r1 = np.rot90(r0)
									r2 = np.rot90(r1)
									r3 = np.rot90(r2)
									r0f = np.fliplr(r0)
									r1f = np.fliplr(r1)
									r2f = np.fliplr(r2)
									r3f = np.fliplr(r3)
									eqr0 = np.array_equal(vari,r0)
									eqr1 = np.array_equal(vari,r1)
									eqr2 = np.array_equal(vari,r2)
									eqr3 = np.array_equal(vari,r3)
									eqr0f = np.array_equal(vari,r0f)
									eqr1f = np.array_equal(vari,r1f)
									eqr2f = np.array_equal(vari,r2f)
									eqr3f = np.array_equal(vari,r3f)
									flag = eqr0 or eqr1 or eqr2 or eqr3 or eqr0f or eqr1f or eqr2f or eqr3f
									"""
									print("possible variations")
									print(r0)
									print(r1)
									print(r2)
									print(r3)
									print(r0f)
									print(r1f)
									print(r2f)
									print(r3f)
									"""
									if flag:
										#print("a match")
										break
									if index == len(poss_pos_count)-1:
										print("possible variations")
										for indexbis in range(len(poss_pos_count)):
											print(poss_pos_count[indexbis])
										print("no match")
										print(vari)
										number += 1
										print("number:", number, "counter:", count)

								
								"""
								tmpfilepos = open("tmpfilepos.txt", "a")
								tmpfilepos.write(vari)
								tmpfilepos.close()
								"""