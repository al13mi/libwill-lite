# WILL
<img src='https://raw.githubusercontent.com/scarsty/neural-demo/master/logo-will.png' width = '10%'/>

WILL is a Neural Net toolkit created by sty@prevt.com, it is an open source code project which belongs to Prevision Limited Company (Hong Kong).

This is the "lite" edition of it, which can establish and train a neural net easily. The "lite" edition supports full-connection, convolution and pooling layers, and some popular active functions such as relu, sigmoid, tanh and softmax. 

More codes will be opened in future.

The codes can be used in education freely, but cannot be used in commercial application without authorizing.

Any questions, please contact with sty@prevt.com or scarsty@gmail.com.

## 简介

### Windows下编译

- 安装Visual Studio 2017（v141），CUDA 9.0（其实可以使用更新的版本）。注意核心库实际上是使用v140编译的，故请自行决定是否安装VS2017的v140开发库，或者你也可以手动升级整个功能。
- 下载cudnn的开发包，将h文件，lib文件和dll文件复制到CUDA工具箱目录中的include，lib/x64和bin目录。或者复制到自己指定的某个目录也可以，但是可能需要自己设置环境变量。
- 请检查环境变量CUDA_PATH的值，通常应该是“C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v9.0”。
- 核心库并不包含图形功能。
- 依赖libopenblas，可以使用msys2安装，或vcpkg。
- 需要nvml库和nvml.dll，请使用nvidia驱动中所带的。
- 下载MNIST的文件放入work/mnist目录，文件名为：t10k-images.idx3-ubyte，t10k-labels.idx1-ubyte，train-images.idx3-ubyte，train-labels.idx1-ubyte。某些解压软件可能解压后中间的.会变成-，请自行修改。
- 编译windows工程，执行```will-windows -c mnist-cv.ini --train```测试效果，正常情况下准确率应在99%以上。
- 你也可以使用FashionMNIST来测试，通过mnist_path选项可以设置不同的文件所在目录。通常测试集上准确率可以到91%左右。
- predict工程为一个供其他程序调用训练好的网络进行计算的接口（lite中不包含）。
- image工程用来合并图片到程序可以读取的格式（lite中不包含）。

### Python模块（lite中不包含）

- 安装swig（建议使用msys2），放在环境变量的可执行目录，或者放在python目录。
- 安装Python 2.7，这里我直接使用了Anaconda。
- 修改Python工程中头文件和链接库文件的目录，通常是%USERPROFILE%\Anaconda2。也可以直接设置环境变量CONDA_PATH。
- 确认Python中自定义包的路径权限为可写和执行。
- 仅能使用Release版。
- 编译成功之后，使用work目录下的jupyter.bat执行范例。
- 打开浏览器，从文档中复制代码运行。

### Linux下编译

#### x86_64

- 请自行解决CUDA和OpenCV的问题，尽量使用高版本。如果自动安装的版本较低，可以删除src/CMakeLists.txt中的链接选项opencv_imgcodecs。
- 下载cudnn，放到/usr/local/cuda，注意lib的目录有可能含有64。
- 注意cuda的默认安装文件夹应该是/usr/local/cuda，但是一些linux发型版可能会安装至其他目录，这时需要修改CMakeLists.txt中的包含目录和链接目录。其中包含有stubs的是nvidia-ml的目录，通常不包含在默认的库目录中，可能需要自行修改。
- 在neural目录下新建```cmake .```生成Makefile。
- ```make```编译，可以加上-j加快速度。
- 生成的可执行文件在bin文件夹。
- 推荐自行建立一个专用于编译的目录，例如```mkdir build```，```cd build```，```cmake ..```，```make```。

#### arm（测试过TK1和TX1）

- TK1只能使用cudnn 2.0，cuda_hack.h和cuda_hack.cpp中通过一些手段将原本的cudnn 5.0的写法转为cudnn 2.0可以使用的形式。仅支持池化、卷积、全连接，激活函数的类型也有限。
- TX1可以使用较新的CUDA版本，编译方法基本一致。
- 推荐使用基于pascal架构的TX2。
