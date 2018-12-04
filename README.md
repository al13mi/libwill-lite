# WILL
<img src='https://raw.githubusercontent.com/scarsty/neural-demo/master/logo-will.png' width = '10%'/>

WILL is a Neural Net toolkit created by sty@prevt.com, it is an open source code project which belongs to Prevision Limited Company (Hong Kong).

This is the "lite" edition of it, which can establish and train a neural net easily. The "lite" edition supports full-connection, convolution and pooling layers, and some popular active functions such as relu, sigmoid, tanh and softmax. 

More codes will be opened in future.

The codes can be used in education freely, but cannot be used in commercial application without authorizing.

Any questions, please contact with sty@prevt.com or scarsty@gmail.com.

## 编译说明

### Windows下编译

- 安装Visual Studio 2017（v141），CUDA 9.2（其实简单修改后可以使用10.0）。注意工程实际上是使用v140，故请自行决定是否安装VS2017的v140开发库，或者也可以手动升级整个工程。
- 下载cudnn的开发包，将h文件，lib文件和dll文件复制到CUDA工具箱目录中的include，lib/x64和bin目录。或者复制到自己指定的某个目录也可以，但是可能需要自己设置环境变量。
- 请检查环境变量CUDA_PATH的值，通常应该是“C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v9.2”。
- 依赖libopenblas，可以使用msys2安装，或vcpkg。
- 需要nvml库和nvml.dll，请使用显卡驱动中自带的，通常dll文件位于“C:\Program Files\NVIDIA Corporation\NVSMI”，该目录一般不在环境变量中，请自行设置或将dll文件复制出来。
- 下载MNIST的文件放入work/mnist目录，文件名为：t10k-images.idx3-ubyte，t10k-labels.idx1-ubyte，train-images.idx3-ubyte，train-labels.idx1-ubyte。某些解压软件可能解压后中间的.会变成-，请自行修改。
- 编译windows工程，执行```will-windows -c mnist-cv.ini --train```测试效果，正常情况下准确率应在99%以上。
- 你也可以使用FashionMNIST来测试，通过mnist_path选项可以设置不同的文件所在目录。通常测试集上准确率可以到91%左右。

### Linux下编译

#### x86_64

- 请自行安装CUDA，尽量使用高版本。
- 下载cudnn，将对应文件放到/usr/local/cuda，或者任何编译器和链接器可以找到的地方。
- 注意cuda的默认安装文件夹应该是/usr/local/cuda，但是一些linux发行版可能会安装至其他目录，这时需要修改CMakeLists.txt中的包含目录和链接目录。其中包含有stubs的是nvidia-ml的目录，通常不包含在默认的库目录中，可能需要自行修改。
- 在neural目录下用cmake生成Makefile。
- 用make命令编译，可以加上-j加快速度。
- 生成的可执行文件在bin文件夹。
- 推荐自行建立一个专用于编译的目录，例如：
```shell
mkdir build
cd build
cmake ..
make
```

#### arm（TK1、TX1、TX2）

- TK1只能使用cudnn 2.0，cuda_hack.h和cuda_hack.cpp中通过一些手段将原本的cudnn 5.0的写法转为cudnn 2.0可以使用的形式。仅支持池化、卷积、全连接，激活函数的类型也有限。

- TX1可以使用较新的CUDA版本，编译方法基本一致。

- 推荐使用基于pascal架构的TX2。

## 完整版与lite版的区别

- 完整版可以进行多维数据的推导与训练，而lite版仅能进行二维数据（即图片，但可以包含多通道）的推导与训练。
- 完整版包含一个仅用于推导的接口，方便部署训练好的网络。
- 完整版包含Python接口。
- 完整版包含基于OpenCV的格式转换功能，但是该功能实际上用处并不大。
- 完整版包含插件功能。
- 完整版的代码较新，布局更合理。