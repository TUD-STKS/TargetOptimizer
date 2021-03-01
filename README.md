# TargetOptimizer
TargetOptimizer is a free and open-source PC software in C++ originally written by Patrick Schmager to estimate pitch targets according to the Target Approximation Model by Yi Xu. 

Hence, it is similar to PENTAtrainer, but differs in the following ways:
- A 5th-order linear system is uses as the filter to approximate the targets instead of a 3rd-order system.
- The target parameters are jointly optimized instead of for one target after another.
- The optimization includes a regularization term to penalize extreme target parameters, e.g., extreme positive or negative target slopes or time constants.
- The estimated targets can be written to a gestural score file for VocalTractLab. This simplifies the copy-synthesis of natural utterances using the articulatory speech synthesizer.

The motivation for these differences are described in Birkholz P, Schmager P, Xu Y (2018). Estimation of Pitch Targets from Speech Signals by Joint Regularized Optimization. In: Proc. of the 26th European Signal Processing Conference (EUSIPCO 2018), pp. 2089-2093, Rome, Italy. [pdf](http://www.vocaltractlab.de/publications/birkholz-2018-eusipco.pdf)

**TargetOptimizer 2.0** is an extension of TargetOptimizer. The performance in terms of minimizing RMSE during estimation of pitch targets has been greatly improved by:
- Making the boundaries a free, fully optimizable parameter (using syllable boundaries as target boundaries does not always lead to optimal estimation result)
- Tuning all free parameters on a test corpus to find optimal default values

Various additions in terms of functionality have been implemented, such as increasing overall performance by adding features like early-stopping during parameter optimization or adding the possibility to work on contours different from pitch contours.


Benchmark results and added features are described in Paul Konstantin Krug, Simon Stone, Alexander Wilbrandt, and Peter Birkholz (2021). Targetoptimizer 2.0: Enhanced estimation of articulatory targets. [pdf](http://www.essv.de/essv2021/pdfs/08_krug.pdf)


The software can be executed as a command line tool (without a GUI, to support batch processing) or as an application with a GUI.

## Build for Windows using Visual Studio 2019+
Simply open the solution TargetOptimizer.sln and build in dependency of desired use case:
1. Command line tool ("Release")
2. GUI ("wxWidgets_Release") when using wxWidgets
3. GUI ("wxWidgets_VCPKG_Release") -> when using VCPKG and wxWidgets

## Build for Linux using GCC:
Navigate inside the Sources folder and run one of the following commands.

For the GUI version (requires wxWidgets):

``g++ -std=c++17 -O3 -D USE_WXWIDGETS -I.. ../dlib/all/source.cpp -fopenmp -fpermissive -lpthread -lX11 *.cpp `wx-config --cxxflags --libs std` -o TargetOptimizer -w -lstdc++fs``

For the command-line-only version:

``g++ -std=c++14 -O3 -I.. ../dlib/all/source.cpp -fopenmp -lpthread -lX11 *.cpp -o TargetOptimizer``

Run TargetOptimizer from the command line:
Run TargetOptimizer -h for instructions.

## Using TargetOptimizer 2.0
Be aware that only a short introdruction can be given at this place. For a more detailed description on how to use TO2 please refer to the manual!

A screenshot of the GUI is shown below (for the German word "Betriebssportgemeinschaft").

![Screenshot Target Optimizer 2.0](doc/TargetOptimizer_GUI.png)

Following steps are necessary to successfully extract targets for an utterance (Step 1 and 2 can be done simultaneously if a TextGrid file is used):
1. Input the boundaries (which can be loaded in terms of a Praat TextGrid file by pressing "Open File(s)" or manually initialized by selecting the amount of desired boundaries and pressing "Init bounds")
2. Input the contour to work on (which can be loaded in terms of a Praat PitchTier file by pressing "Open File(s)")
3. Carefully think about the parameters you want to change (as mentioned above, default values are generally well suited)
4. Press "Optimize Targets"
5. Export the results as a CSV file, a gestural score for VocalTractLab, or as a Praat PitchTier file with the model f0 contour by pressing "Save as..."
