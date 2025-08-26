from folder_handlers import create_folder
from viewer_mc_kawasaki import view_kawasaki,time_kawasaki,process_json
from viewer_mc_hysteresis import view_hysteresis,time_hysteresis
from viewer_mc_classic import view_classic,time_classic

filenames_folder = {
	"../out_files/mc_kawasaki (n=200, steps=3000).json":"official"
}

rdata = [2,7,1] # [1,6,1] [6,9,1] range(100,201,50)
rlow,rhigh,rstep = rdata
for filename in filenames_folder.keys():
	folder = filenames_folder[filename]
	create_folder(f"../png_files/{folder}/")
	create_folder(f"../png_files/{folder}/kawasaki")
	create_folder(f"../png_files/{folder}/kawasaki/f")
	create_folder(f"../png_files/{folder}/kawasaki/m")
	create_folder(f"../png_files/{folder}/kawasaki/c")
	create_folder(f"../png_files/{folder}/kawasaki/a")
	create_folder(f"../png_files/{folder}/kawasaki/t")
	time_kawasaki(filename,folder=folder)
	res,Clist,Tlist = process_json(filename,rdata)
	for mode in ["f","m","c","a"]: # 
		view_kawasaki(filename,res,Clist,Tlist,folder=folder,mode=mode)

filenames_folder = {
	"../out_files/mc_hysteresis (n=200, muinfo=(-0.690000,-0.655000,400), steps=100).json":"official"
}

for filename in filenames_folder.keys():
	folder = filenames_folder[filename]
	create_folder(f"../png_files/{folder}/")
	create_folder(f"../png_files/{folder}/hysteresis")
	time_hysteresis(filename,folder=folder)
	view_hysteresis(filename,folder=folder)

filenames_folder = {
	"../out_files/mc_classic (n=200, muinfo=(-1.000000,-0.250000,200), steps=100).json":"official"
}

dropkeys = ["400.000000","500.000000","700.000000","800.000000","900.000000"]

for filename in filenames_folder.keys():
	folder = filenames_folder[filename]
	create_folder(f"../png_files/{folder}/")
	create_folder(f"../png_files/{folder}/classic")
	time_classic(filename,folder=folder)
	view_classic(filename,dropkeys,code="a",folder=folder)
	view_classic(filename,[],code="b",folder=folder)
	view_classic(filename,[],code="c",folder=folder)