# Aipic-V1.0   
# **基于深度学习的智能图像分类系统**

**前提条件**
> 
    1. Caffe C++ 接口，或者 OpenCV dnn 模块。（推荐使用前者，更方便灵活） 
    2. VS2015 编译器，这是由于OpenCV3.2对C++11特性的要求
    3. OpenCV3.2 with opencv_contrib 

**软件功能**
> 
    1.图像文件批量导入和浏览
    2.图像特征提取和可视化： SIFT SURF ORB LBP HOG Hist
    3.Hand-Crafted分类器训练： 分类器支持 SVM、KNN ，特征支持： ORB SIFT SURF（BOW处理，CPU计算）
    4.基于Caffe的CNN图像分类（GPU计算）
    5.TODO：Caffe CNN feature vision

**软件使用教程**

请看这里：[How to use][1]

**软件界面**

![GUI][2]


![Feature Extract][3]


![Train Classifier][4]


![Image Classify][5]


  [1]: http://blog.csdn.net/muyouhang/article/details/73612153
  [2]: http://img.blog.csdn.net/20170622233208994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXV5b3VoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast
  [3]: http://img.blog.csdn.net/20170622233618063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXV5b3VoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast
  [4]: http://img.blog.csdn.net/20170622233310642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXV5b3VoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast
  [5]: http://img.blog.csdn.net/20170622233746140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXV5b3VoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast
