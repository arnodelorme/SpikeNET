1 - Install
=====

SpikeNet is compiled by default for OSX 64 bit and Linux Fedora core

To recompile SpikeNET. Edit the Makefile file to change the location of the X11 librairies (type "locate X11" on your command line prompt). SpikeNET intentionnaly uses very few external librairies.

	cd src
	make clean
	make
	cd ..
	
 This creates an executable named "SpikeNET".

2 - Run Demo
=====

In the main SpikeNET folder type one of the following depending of your platform

	./SpikeNET
	./SpikeNET_linux
	./SpikeNET_osx


3 - Directory content
=====

Convolution (connexion) file folders for demo and exemples
----------------------------------------------------------
	Convolutions
	convos40Faces
	FaceConvos
	MSFaceConvos

Image folder
------------
	Images

SpikeNET demo and example model directories
-------------------------------------------
	networkdemo40faceslearn
	networkdemo40facesrandom
	networkdemo40facesrun
	networkdemodetection
	networkdemodetectionms
	network_example_4faceslearn
	network_example_4facesoptimize
	network_example_orientation


SpikeNET convolution output directory
-------------------------------------
	save_convos

SpikeNET source directory
-------------------------
	SpikeNETsources

SpikeNET GNU license
--------------------
	license.txt


Arnaud Delorme, February, 17, 2021
