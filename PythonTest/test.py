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
kivy.require('1.0.6')

from glob import glob
from random import randint
from os.path import join, dirname
from kivy.app import App
from kivy.logger import Logger
from kivy.uix.scatter import Scatter
from kivy.properties import StringProperty

import numpy as np

from PIL import Image
from skimage import io,data,filters
import cv2
import matplotlib.pyplot as plt

#泛洪填充，相似像素填充
def fill_color(img):
    h,w,t=img.shape
    #必要参数
    maskx=np.zeros([h+2,w+2],np.uint8)
    #参数接收：（图片信息，必要参数，参考点位置坐标，填充的颜色，查找范围：最低像素（参考减所写），查找范围：最高像素（参考加所写），全部填充）
    cv2.floodFill(img,maskx,(150,200),(255,255,0),(100,100,100),(50,50,50),cv2.FLOODFILL_FIXED_RANGE)
    return img
    # cv2.imshow("area",img)

class Picture(Scatter):

    source = StringProperty(None)

class PicturesApp(App):

    def build(self):
        pil_im = Image.open("./images/th.jpg")
        print(pil_im.size,pil_im.format,pil_im.mode)
        # pil_im.resize(200,200)
        # pil_im.save("./images/th.jpg") 
        # pil_im = Image.open("./images/th.jpg")
        # print(pil_im.size,pil_im.format,pil_im.mode)

        # the root is created in pictures.kv
        root = self.root

        img1=np.array(Image.open('./images/lean.jpg'))  #打开当前目录图像并转化为数字矩阵
        img = fill_color(img1)
        plt.figure("lena")
        plt.imshow(img)
        plt.axis('off')
        plt.show()
        
        print(img.shape)  
        print(img.dtype) 
        print(img.size) 
        print(type(img))
        
        # get any files into images directory
        curdir = dirname(__file__)
        for filename in glob(join(curdir, 'images', '*')):
            try:
                # load the image
                picture = Picture(source=filename, rotation=0)
                # add to the main field
                root.add_widget(picture)
            except Exception as e:
                Logger.exception('Pictures: Unable to load <%s>' % filename)

    def on_pause(self):
        return True

if __name__ == '__main__':
    PicturesApp().run()