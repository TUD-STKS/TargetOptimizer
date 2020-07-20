# TargetOptimizer
TargetOptimizer is a free and open-source PC software in C++ originally written by Patrick Schmager to estimate pitch targets according to the Target Approximation Model by Yi Xu. 

Hence, it is similar to PENTAtrainer, but differs in the following ways:
- A 5th-order linear system is uses as the filter to approximate the targets instead of a 3rd-order system.
- The target parameters are jointly optimized instead of for one target after another.
- The optimization includes a regularization term to penalize extreme target parameters, e.g., extreme positive or negative target slopes or time constants.
- The estimated targets can be written to a gestural score file for VocalTractLab. This simplifies the copy-synthesis of natural utterances using the articulatory speech synthesizer.

The motivation for these differences are described in Birkholz P, Schmager P, Xu Y (2018). Estimation of Pitch Targets from Speech Signals by Joint Regularized Optimization. In: Proc. of the 26th European Signal Processing Conference (EUSIPCO 2018), pp. 2089-2093, Rome, Italy. [pdf](http://www.vocaltractlab.de/publications/birkholz-2018-eusipco.pdf)

To estimate the pitch targets for an utterance, the model needs as input the syllable boundaries in terms of a Praat TextGrid file, and the pitch contour in terms of a Praat PitchTier file. Based on these data, the pitch targets can be estimated and exported in terms of a CSV file, a gestural score for VocalTractLab, or as a Praat PitchTier file with the model f0 contour. The software can be executed as a command line tool (without a GUI, to support batch processing) or as an application with a GUI. To call the program with GUI, simply start it without any parameters. A screenshot of the GUI is shown below (for the German word "Ästhetik").

![Screenshot Target Optimizer 1.0](http://www.vocaltractlab.de/pictures/targetoptimizer-screenshot.gif)

## Build for Windows using Visual Studio 2019+
Simply open the solution TargetOptimizer.sln and build.


## Build for Linux using GCC:

Navigate inside the Sources folder and run:

g++ -std=c++11 -O3 -I.. ../dlib/all/source.cpp -fopenmp -lpthread -lX11 *.cpp -o TargetOptimizer


Run TargetOptimizer from the command line:
Run TargetOptimizer -h for instructions.
