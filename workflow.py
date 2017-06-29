
import os
import datetime

print("Welcome to the AQUA smart workflow.\n")

def update():
	if os.system("git pull origin master") == 0: print("Source files updated with success.")
	else: print("An error occured whilst updating your source files.")

def maintain():
	print("Running maintnance check ...")
	os.system("find include/ -type f -exec sed -i 's/%d\ \/\/\/\ !!!\ Change\ this\ every\ year/%d\ \/\/\/\ !!!\ Change\ this\ every\ year/g' {} +"%(datetime.datetime.now().year - 1, datetime.datetime.now().year))
	
	os.system("find include/ -type f -exec sed -i 's/Copyright\ %d\ Obiwac/Copyright\ %d\ Obiwac/g' {} +"%(datetime.datetime.now().year - 1, datetime.datetime.now().year))
	os.system("find src/ -type f -exec sed -i 's/Copyright\ %d\ Obiwac/Copyright\ %d\ Obiwac/g' {} +"%(datetime.datetime.now().year - 1, datetime.datetime.now().year))
	os.system("find asm/ -type f -exec sed -i 's/Copyright\ %d\ Obiwac/Copyright\ %d\ Obiwac/g' {} +"%(datetime.datetime.now().year - 1, datetime.datetime.now().year))
	os.system("find build/ -type f -exec sed -i 's/Copyright\ %d\ Obiwac/Copyright\ %d\ Obiwac/g' {} +"%(datetime.datetime.now().year - 1, datetime.datetime.now().year))
	
	version = float(open("version.ver").read())
	os.system("find build/ -type f -exec sed -i 's/%g\"\ #\ dont\ forget\ to\ change\ the\ version\ number/%g\"\ #\ dont\ forget\ to\ change\ the\ version\ number/g' {} +"%(version - 0.1, version))
	
	os.system("find asm/ -type f -exec sed -i 's/1;MARKER_TEXT_OR_VIDEO_MODE_SWITCH/0;MARKER_TEXT_OR_VIDEO_MODE_SWITCH/g' {} +")

if not raw_input("Do you want to update your source files and run the maintnance check? ") in ["no", "No", ""]:
	update()
	maintain()

while True:
	command = raw_input("\n> ").lower()
	
	if command == "update": update()
	elif command == "maintain": maintain()
	elif command == "version": os.system("cat version.ver")
	
	elif command == "set version":
		os.system("echo %s > version.ver"%(raw_input("Enter your new version name: ")))
		maintain()
	
	elif command in ["setup", "geany"]:
		if os.system("geany include/types.h") == 0: print("Geany enviroment setup with success.")
		else: print("An error occured whilst setting up the Geany enviroment.")
	
	elif command == "commit":
		if os.system("git add .") == 0:
			print("Source files added to repository list with success.")
			
			if os.system("git commit -a -m \"%s\""%(raw_input("Enter commit message: "))) == 0:
				print("Commit made with success.")
				update()
				
				if os.system("git push origin master") == 0: print("Repository updated with success.")
				else: print("An error occured whilst updating the repository.")
			
			else:
				print("An error occured whilst commiting you source files.")
		
		else:
			print("An error occured whilst adding your source files.")
	
	elif command == "text mode":
		if os.system("find asm/ -type f -exec sed -i 's/0;MARKER_TEXT_OR_VIDEO_MODE_SWITCH/1;MARKER_TEXT_OR_VIDEO_MODE_SWITCH/g' {} +") == 0: print("Switched video mode to text mode with success.")
		else: print("An error occured whilst switching modes.")
	
	elif command == "video mode":
		if os.system("find asm/ -type f -exec sed -i 's/1;MARKER_TEXT_OR_VIDEO_MODE_SWITCH/0;MARKER_TEXT_OR_VIDEO_MODE_SWITCH/g' {} +") == 0: print("Switched text mode to video mode with success.")
		else: print("An error occured whilst switching modes.")
	
	elif command in ["compile", "build"]:
		if os.system("make build") == 0: print("AQUA compiled with success.")
		else: print("An error occured whilst compiling AQUA.")
	
	elif command == "make":
		if os.system("make iso") == 0: print("AQUA made with success.")
		else: print("An error occured whilst making AQUA.")
	
	elif command == "run":
		os.system("(killall virtualbox && sleep 1) || true")
		
		if os.system("virtualbox --startvm \"AQUA OS\"") == 0: print("Ran AQUA with success.")
		else: print("An error occured whilst running AQUA.")
	
	elif command in ["new", "create"]:
		_type = raw_input("What file type do you want to create? ")
		name = raw_input("What's the file name? ")
		
		path = ""
		include = ""
		deps = ""
		
		if _type in ["driver", "drv"]:
			name = "%s_driver"%(name)
			path = "drivers/"
		
		elif _type in ["application", "app"]:
			include = """#include "../../types.h"
#include "api.h" """
			
			print("What assets do you want to include?")
			
			while True:
				asset = raw_input(">> ")
				
				if asset == "icon off": deps = deps + "\tuint8* _api_icon_off = api_get_icon(\"off\")\n"
				elif asset == "icon back": deps = deps + "\tuint8* _api_icon_back = api_get_icon(\"back\")\n"
				elif asset == "icon save": deps = deps + "\tuint8* _api_icon_save = api_get_icon(\"save\")\n"
				elif asset == "icon load": deps = deps + "\tuint8* _api_icon_load = api_get_icon(\"load\")\n"
				elif asset == "icon next": deps = deps + "\tuint8* _api_icon_next = api_get_icon(\"next\")\n"
				
				elif asset == "font aqua 20": deps = deps + "\tuint8** _api_font_aqua_20px = api_get_font(\"aqua\", 20)\n"
				elif asset == "font aqua 50": deps = deps + "\tuint8** _api_font_aqua_50px = api_get_font(\"aqua\", 50)\n"
				elif asset == "font mono 15": deps = deps + "\tuint8** _api_font_aqua_15px = api_get_font(\"mono\", 15)\n"
				
				else: break
			
			_type = "app"
			name = "appliction_%s"%(name)
			path = "user/applications/"
		
		_file = open("include/%s/%s.h"%(path, name), "w")
		_file.write("""/*
 * %s.h
 * 
 * Copyright %d Obiwac
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#ifndef %s_H
	#define %s_H
	
%s
	
#endif"""%(name, datetime.datetime.now().year, name.upper(), name.upper(), include))
		
		_file.close()
		_file = open("src/%s/%s.c"%(path, name), "w")
		
		if _type == "app":
			_file.write("""/*
 * %s.c
 * 
 * Copyright %d Obiwac
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "%s.h"

void launch_%s(void) {
	%s
	
}"""%(name, datetime.datetime.now().year, name, name))
		
		else:
			_file.write("""/*
 * %s.c
 * 
 * Copyright %d Obiwac
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "%s.h" """%(name, datetime.datetime.now().year, name))

		_file.close()
		
		os.system("geany src/%s/%s.c"%(path, name))
		os.system("geany include/%s/%s.h"%(path, name))
		os.system("geany Makefile")
		
		if _type == "app":
			os.system("geany src/user/app_launcher.c")
			os.system("gaeny src/OS.c")
		
		else:
			os.system("gaeny include/OS.h")
	
	elif command == "open":
		_type = raw_input("What file type do you want to open? ")
		name = raw_input("What's the file name? ")
		
		if _type in ["driver", "drv"]:
			os.system("geany src/drivers/%s_driver.c"%(name))
			os.system("geany include/drivers/%s_driver.h"%(name))
		
		elif _type in ["c", "src", "source"]:
			os.system("geany src/%s.c"%(name))
		
		elif _type in ["h", "header", "inc", "include"]:
			os.system("geany include/%s.h"%(name))
		
		elif _type in ["asm", "assembly"]:
			os.system("geany asm/%s.asm"%(name))
		
		else:
			os.system("geany %s"%(name))
	
	elif command == "release":
		if not raw_input("Are you sure you updated the version number? ") == "":
			maintain()
	
	elif command == "tree": os.system("tree")
	elif command in ["tree src", "tree source", "tree c"]: os.system("tree src/")
	elif command in ["tree inc", "tree include", "tree h", "tree header"]: os.system("tree include/")
	elif command in ["tree asm", "tree assembly"]: os.system("tree asm/")
	elif command in ["tree drv", "tree drivers"]: os.system("tree src/drivers/")
	
	elif command in ["tree sub", "tree dir", "tree directory"]:
		if os.system("tree %s"%(raw_input("Enter the path that you want to tree: "))) == 0: print("Path found and scanned with success.")
		else: print("An error occured whilst displaying a tree for the entered path.")
	
	elif command in ["linux", "cmd", "command", "system", "shell"]:
		if os.system(raw_input("Enter your command: ")) == 0: print("Command ran with success.")
		else: print("An error occured whilst running command.")
	
	elif command in ["quit", "exit", "bye"]:
		print("Goodbye...")
		os.system("killall geany")
		break
	
	else:
		print("This command is unknown to me.")

quit()
exit()
