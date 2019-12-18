'''
Basic Picture Viewer
====================
This simple image browser demonstrates the scatter widget. You should
see three framed photographs on a background. You can click and drag
the photos around, or multi-touch to drop a red dot to scale and rotate the
photos.
The photos are loaded from the local images directory, while the background
picture is from the data shipped with kivy in kivy/data/images/background.jpg.
The file pictures.kv describes the interface and the file shadow32.png is
the border to make the images look like framed photographs. Finally,
the file android.txt is used to package the application for use with the
Kivy Launcher Android application.
For Android devices, you can copy/paste this directory into
/sdcard/kivy/pictures on your Android device.
'''

import cv2
import numpy as np

#读取图像，支持 bmp、jpg、png、tiff 等常用格式
image = cv2.imread("F://project//Learning//PythonTest//images//lean.jpg",cv2.IMREAD_COLOR)

print(image.shape)  #输出：(1080, 1920, 3) 高像素，宽像素，通道数
print(image.size)   # 120000  总通道数=高* 宽* 通道数
print(image.dtype)  # uint8  3个通道每个通道占的位数（8位，一个字节）
print(np.array(image))   #输出每个像素点的参数（ B , G , R )

#获取图片 B G R 各均值， #(204.46305102040816, 208.50832244897958, 217.29540408163263, 0.0) ，红色部分最多
print(cv2.mean(image))
#获取方差,也会打印均值，可用均值方差都为零判断图片无效
print(cv2.meanStdDev(image))

#创建窗口并显示图像
# cv2.namedWindow("Image")
image = cv2.flip(image,0)

#.........................................................................
#图片处理
#备份图片
imgviewx1=image.copy()

#均值模糊,主要用于去除图片噪点
#读取图片并实现图片的模糊效果，参数：（读取图片，（X轴方向模糊，Y轴方向模糊））
imgviewx=cv2.blur(imgviewx,(5,5))

#中值模糊，主要用于去除椒盐（烧烤配料）噪点
#参数：（图片信息，模糊值）
#mgviewx=cv2.medianBlur(imgviewx,9)

#普通高斯模糊
#参数：（图片信息，参数1，参数2）参数1和参数2只能设置一个
imgviewx=cv2.GaussianBlur(imgviewx,(0,0),1)

#保留边缘（像素差），高斯模糊
#参数（图片信息，0，要用怎样的方式（越大则越细），空间复杂度（越大越复杂））
imgviewx=cv2.bilateralFilter(imgviewx,0,50,6)

#美颜,美白效果valuex值越大越白
#valuex=50;
#imgviewx=cv2.bilateralFilter(cv2.imread("imgx/zcy.jpg"),valuex,valuex * 2,valuex / 2)

#对比度和亮度调整
#duix=0.5 #对比度
#lightx=0  #亮度
#imgviewx=cv2.addWeighted(imgviewx,duix,np.zeros(imgviewx.shape,imgviewx.dtype),1-duix,lightx)

cv2.imshow("Image xixixi",image)

cv2.imwrite("F://project//Learning//PythonTest//abc.png",image, [int( cv2.IMWRITE_PNG_COMPRESSION), 5])

cv2.waitKey(0)

#释放窗口
cv2.destroyAllWindows()

