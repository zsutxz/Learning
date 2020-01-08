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
The images in the image directory are from the Internet Archive,
`https://archive.org/details/PublicDomainImages`, and are in the public
domain.
'''

import kivy
kivy.require('1.9.0')

from glob import glob
from random import randint
from os.path import join, dirname
from kivy.app import App
from kivy.logger import Logger
from kivy.uix.scatter import Scatter
from kivy.properties import StringProperty

import numpy as np
import matplotlib.pyplot as plt

import cv2

# from PIL import Image
# from skimage import io,data,filters

class Picture(Scatter):

    source = StringProperty(None)

class PicturesApp(App):
    def ch_detect(self,img):
        # 读取文件
        # imagePath = '/Users/amen/data/Learning/PythonTest/images/th.jpg'
        # image = cv2.imread(imagePath)
        cv2.imshow("orign", img) #展示图片

        retval,tresh=cv2.threshold(img,136,255,cv2.THRESH_BINARY)
        cv2.imshow("tresh", tresh) #展示图片

        #灰度图片
        gray = cv2.cvtColor(tresh, cv2.COLOR_BGR2GRAY)
        # cv2.imshow("gray", gray) #展示图片

        #二值化
        binary = cv2.adaptiveThreshold(~gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 43, -33)

        cv2.imshow("binary2", binary) #展示图片
        # cv2.waitKey(0)

        rows,cols=binary.shape
        scale = 25
        #识别横线
        kernel  = cv2.getStructuringElement(cv2.MORPH_RECT,(cols//scale,1))
        eroded = cv2.erode(binary,kernel,iterations = 1)
        #cv2.imshow("Eroded Image",eroded)
        dilatedcol = cv2.dilate(eroded,kernel,iterations = 1)
        # cv2.imshow("cols:",dilatedcol)

        #识别竖线
        scale = 20
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(1,rows//scale))
        eroded = cv2.erode(binary,kernel,iterations = 1)
        dilatedrow = cv2.dilate(eroded,kernel,iterations = 1)
        # cv2.imshow("rows:",dilatedrow)
        
        #标识表格
        merge = cv2.add(dilatedcol,dilatedrow)
        # cv2.imshow("merge0:",merge)

        #两张图片进行减法运算，去掉表格框线
        merge2 = cv2.subtract(binary,merge)
        return merge2
        # cv2.imshow("merge2:",merge2)
        
    def build(self):

        root = self.root
        
        # get any files into images directory
        curdir = dirname(__file__) 
        
        # img = np.array(Image.open(curdir+'/images/lean.jpg'))  #打开当前目录图像并转化为数字矩阵
    
        # plt.figure("lena")
        # plt.imshow(img)
        # plt.axis('off')
        # plt.show()
        
        # 读取文件
        imagePath = curdir+'/images/th.jpg'
        image = cv2.imread(imagePath)
        
        image = self.ch_detect(image)
        leanPath = curdir+'/images/lean.jpg'
        # for filename in glob(join(curdir, 'images', '*')):
        try:
            
            # load the image
            picture = Picture(source=imagePath, rotation=0)
            # add to the main field
            root.add_widget(picture)
        except Exception as e:
            Logger.exception('Pictures: Unable to load <%s>' % imagePath)

    def on_pause(self):
        return True

if __name__ == '__main__':
    PicturesApp().run()