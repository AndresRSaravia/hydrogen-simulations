import os

def create_folder(folder):
	try:
	    os.mkdir(folder)
	    print(f"'{folder}' created successfully")
	except FileExistsError:
	    print(f"'{folder}' already exists")
	except PermissionError:
	    print(f"permission denied: unable to create '{folder}'")
	except Exception as e:
	    print(f"an error occurred: {e}")
