SpikeNET
===
This project is no longer actively supported, although the code is functional.

SpikeNET is a program designed for simulating large networks of asynchronous spiking neurons. Neurons are simulated with a limited number of parameters that include classic properties like the post-synaptic potential and threshold and more novel features like dendritic sensitivity. SpikeNET can be used to simulate networks with millions of neurons and hundreds of millions of synaptic weights. Optimization of computation time and the aim of real-time computation has been one of the driving forces behind the development of SpikeNET.

What SpikeNET can do
----
* Perform image processing using a biologically plausible network of neurons.
* Simulate millions of integrate-and-fire neurons organized in retinotopical maps.
* Connect these neuronal maps using projection files, and regroup common synaptic weights to save memory in order to be able to declare several hundreds of billions of synaptic connections.
* Convert gray level images into lists of spikes (also SpikeNET can perform a variety of preprocessing on the input images).
* Implement a complex mechanism for projection between neuronal maps of different sizes.
* Implement supervised learning.
* Implement the efficient neuronal Rank-Order-Coding scheme (optional)

What SpikeNET cannot  do
----
* SpikeNET does not have a comprehensive graphic interface. The neural network topology must be described using several configuration files (documented below).
* SpikeNET has never been used to process more than one spike per neuron (SpikeNET was initially designed to test the biological plausibility of feed-forward processing using at most one spike per neuron). Note that it is still possible to implement lateral inhibition or excitation and feedback as long as neurons discharge only once.
* SpikeNET can not implement synaptic connections with various delays. All synaptic connections are instantaneous.

A glimpse at SpikeNET performances 
---
See this [page for more information](http://arnauddelorme.com/neural-network-2/).

When we tested the performance of SpikeNET in 1999 rigorously, SpikeNET could compute roughly 20 million connections per second on a standard desktop computer (Macintosh Power PC 233 Mhz). At this time, we believe that SpikeNET was the most powerful convolutional neural network for image classification available.

Publications
====
Publications directly related to SpikeNET
---
* Delorme, A., Thorpe, S. (2003). SpikeNET: An Event-driven Simulation Package for Modeling Large Networks of Spiking Neurons, <i>Network: Comput. Neural Syst.</i>, 14, 613:627. <a
 href="https://sccn.ucsd.edu/~arno/mypapers/delormeN2003.pdf" target="_blank">Author's PDF</a>, <a href="http://www.iop.org/EJ/article/0954-898X/14/4/301/ne3401.pdf" target="_blank">journal's link</a>. 
* Delorme, A., Thorpe, S. (2001) Face processing 
using one spike per neuron: resistance to image degradation. <i>Neural Networks</i>,&nbsp;
14(6-7), 795-804. <a href="https://sccn.ucsd.edu/~arno/mypapers/DelormeNN2001.pdf" target="_blank">Author's PDF</a>, <a href="http://www.sciencedirect.com/science?_ob=ArticleURL&_udi=B6T08-4475J9D-M&_user=4429&_handle=W-WA-A-A-BW-MsSAYWA-UUA-AUZUZEBEEU-AYVZECZDE-BW-U&_fmt=full&_coverDate=07%2F09%2F2001&_rdoc=18&_orig=browse&_srch=%23toc%234856%232001%23999859993%23268145!&_cdi=4856&view=c&_acct=C000000152&_version=1&_urlVersion=0&_userid=4429&md5=ffaff470b70c6ecfc6bede3dce4c3f15" target="_blank">Science Direct</a>. </p>
* Delorme, A., Gautrais, J., VanRullen, R., 
&amp; Thorpe, S.J. (1999). SpikeNET: A simulator for modeling large networks 
of integrate and fire neurons. <i>Neurocomputing</i>, 26-27, 989-996. <a
 href="https://sccn.ucsd.edu/~arno/mypapers/Delorme99.pdf">Author's PDF</a>, <a href="http://www.sciencedirect.com/science?_ob=GatewayURL&_origin=compsciportal&_method=citationSearch&_urlversion=4&_version=1&_piikey=S0925231299000958&md5=bfcb0ba39bf9beaf840e220ad7601923" target="_blank">Science Direct</a>.
* Van Rullen, R., Gautrais, J., Delorme, A.,
&amp; Thorpe, S. (1998). Face processing using one spike per neurone. <i>Biosystems</i>, 
48(1-3), 229-239. <a href="https://sccn.ucsd.edu/~arno/mypapers/VanRullen98.pdf" target="_blank">Author's PDF</a>, <a href="http://www.sciencedirect.com/science?_ob=ArticleURL&_udi=B6T2K-3V5V00M-10&_user=4429&_handle=W-WA-A-A-BD-MsSAYVA-UUW-AUZUDDDCZV-AYCBVVCAE-BD-U&_fmt=full&_coverDate=11%2F01%2F1998&_rdoc=28&_orig=browse&_srch=%23toc%234921%231998%23999519998%2332734!&_cdi=4921&view=c&_acct=C000000152&_version=1&_urlVersion=0&_userid=4429&md5=c93cd51c8e5f5f2b9859484bb831bb25" target="_blank">Science Direct</a>.

Publications indirectly related to SpikeNET
---
* Delorme, A. (2003) Early Cortical Orientation Selectivity:
How Fast Shunting Inhibition Decodes the Order of Spike Latencies. <i>Journal of Computational Neuroscience</i>, 15, 357-365. <a href="https://sccn.ucsd.edu/~arno/mypapers/Delorme2003.pdf" target="_blank">Author's PDF</a>, <a href="http://ipsapp008.kluweronline.com/content/getfile/4835/47/5/fulltext.pdf" target = "_blank">journal's link</a>.
* Thorpe, S., Delorme, A., VanRullen, R. (2001)
Spike based strategies for rapid processing. <i>Neural Networks</i>, 14(6-7),
715-726. <a href="https://sccn.ucsd.edu/~arno/mypapers/ThorpeSpiking_Neurons.pdf">Author's PDF</a>, <a href="http://www.sciencedirect.com/science?_ob=ArticleURL&_udi=B6T08-4475J9D-D&_user=4429&_handle=W-WA-A-A-BW-MsSAYWA-UUA-AUZUZEBEEU-AYVZECZDE-BW-U&_fmt=full&_coverDate=07%2F09%2F2001&_rdoc=12&_orig=browse&_srch=%23toc%234856%232001%23999859993%23268145!&_cdi=4856&view=c&_acct=C000000152&_version=1&_urlVersion=0&_userid=4429&md5=b3b30bd118bd596deea48e46ff888af8" target="_blank">Science Direct</a>.
* Perrinet, L., Delorme, A., Thorpe, S. (2001) 
Network of integrate-and-fire neurons using Rank Order Coding A: how to implement
spike timing dependant plasticity. <i>Neurocomputing</i>, 38-40(1-4), 817-822. 
  <a href="https://sccn.ucsd.edu/~arno/mypapers/Perinnet.cns200.pdf" target="_blank">Author's PDF</a>, <a href="http://www.sciencedirect.com/science?_ob=GatewayURL&_origin=compsciportal&_method=citationSearch&_urlversion=4&_version=1&_piikey=S092523120100460X&md5=e8cc5e653f0720321ebf717bffa35031" target="_blank">Science Direct</a>.
* Delorme, A., Perrinet, L., Thorpe, S. (2001) 
Network of integrate-and-fire neurons using Rank Order Coding B: spike timing
dependant plasticity and emergence of orientation selectivity. <i>Neurocomputing</i>, 
38-40(1-4), 539-545. <a href="https://sccn.ucsd.edu/~arno/mypapers/Delorme.cns2000.pdf" target="_blank">Author's PDF</a>, <a href="http://www.sciencedirect.com/science?_ob=GatewayURL&_origin=compsciportal&_method=citationSearch&_urlversion=4&_version=1&_piikey=S0925231201004039&md5=cbeaa7597d8ada435436ec59face748d" target="_blank">Science Direct</a>.</font> 
* VanRullen, R., Delorme, A. &amp; Thorpe 
S.J. (2001). Feed-forward contour integration in primary visual cortex based
on asynchronous spike propagation. <i>Neurocomputing</i>, 38-40(1-4), 1003-1009. 
  <a href="https://sccn.ucsd.edu/~arno/mypapers/ContourIntegration.PDF" target="_blank">Author's PDF</a>, <a href="http://www.sciencedirect.com/science?_ob=GatewayURL&_origin=compsciportal&_method=citationSearch&_urlversion=4&_version=1&_piikey=S0925231201004453&md5=ee6bdc771e75cf723370f2b8803b9742" target="_blank">Science Direct</a>.
* Thorpe, S.J., Delorme, A., VanRullen, R., Paquier, W. (2000) Reverse engineering of the visual system using networks  of spiking neurons. <i>IEEE International Symposium on Circuits and 
Systems, 2000, </i>4, 405 -408. <a href="https://sccn.ucsd.edu/~arno/mypapers/thorpe.pdf" target="_blank">Author's PDF</a>, <a href="http://ieeexplore.ieee.org/iel5/6910/18613/00858774.pdf?isNumber=18613&prod=CNF&arnumber=858774&arSt=405&ared=408+vol.4&arAuthor=Thorpe%2C+S.J.%3B+Delorme%2C+A.%3B+Van+Rullen%2C+R.%3B+Paquier%2C+W.%3B" target="_blank">journal's link</a>.</font> 
* Delorme, A., Gautrais, J., VanRullen, R., 
&amp; Thorpe, S.J. (1999). SpikeNET: A simulator for modeling large networks 
of integrate and fire neurons. <i>Neurocomputing</i>, 26-27, 989-996. <a  href="https://sccn.ucsd.edu/~arno/mypapers/Delorme99.pdf">Author's PDF</a>, <a href="http://www.sciencedirect.com/science?_ob=GatewayURL&_origin=compsciportal&_method=citationSearch&_urlversion=4&_version=1&_piikey=S0925231299000958&md5=bfcb0ba39bf9beaf840e220ad7601923" target="_blank">Science Direct</a>.

Using SpikeNET
====
1 - Install
---
SpikeNet is compiled by default for OSX 64 bit and Linux Fedora core.

To recompile SpikeNET. Edit the Makefile file to change the location of the X11 librairies (type "locate X11" on your command line prompt). SpikeNET intentionnaly uses very few external librairies.

	cd src
	make clean
	make
	cd ..
	
 This creates an executable named "SpikeNET".

2 - Run Demo
---

In the main SpikeNET folder type one of the following depending of your platform

	./SpikeNET
	./SpikeNET_linux
	./SpikeNET_osx


3 - Directory content
---
Convolution (connexion) file folders for demo and exemples
	Convolutions
	convos40Faces
	FaceConvos
	MSFaceConvos

Image folder
	Images

SpikeNET demo and example model directories
	networkdemo40faceslearn
	networkdemo40facesrandom
	networkdemo40facesrun
	networkdemodetection
	networkdemodetectionms
	network_example_4faceslearn
	network_example_4facesoptimize
	network_example_orientation


SpikeNET convolution output directory
	save_convos

SpikeNET source directory
	SpikeNETsources

SpikeNET GNU license
	license.txt


Arnaud Delorme, February, 17, 2021
