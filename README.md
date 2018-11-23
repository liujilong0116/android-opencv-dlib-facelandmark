# android-opencv-dlib-facelandmark
这是我写好的第一个结合opencv以及dlib在Android平台实现人脸68个特征点的检测。 上传代码的目的主要是保存一下关于cmakelists里的配置，按照网上的说法试了很久才搞定的，也提供给需要的人去下载。 因为写的比较匆忙，没来的打上一些注释，下面简单描述一下功能的实现。 重点是环境的配置，这个程序也就是实现了一个简单的功能。 使用opencv的人脸检测模块找到人脸的区域，将opencv的Rect格式的矩形转换成dlib的rectangle，再用68点的模型进行标记。 只是一个简单的测试，用于学习。

使用时，需开启应用的内存读写权限，不然无法从内存中加载文件。

效果如下：

![示例](https://github.com/liujilong0116/android-opencv-dlib-facelandmark/blob/master/image/1.jpg)
