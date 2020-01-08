
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

# Load two images
img1 = cv2.imread('/Users/amen/data/Learning/PythonTest/images/lean.jpg')
img2 = cv2.imread('/Users/amen/data/Learning/PythonTest/images/logo.png')

# I want to put logo on top-left corner, So I create a ROI
rows,cols,channels = img2.shape
roi = img1[0:rows, 0:cols ]

# Now create a mask of logo and create its inverse mask also
img2gray = cv2.cvtColor(img2,cv2.COLOR_BGR2GRAY)
ret, mask = cv2.threshold(img2gray, 10, 255, cv2.THRESH_BINARY)
mask_inv = cv2.bitwise_not(mask)

# Now black-out the area of logo in ROI
img1_bg = cv2.bitwise_and(roi,roi,mask = mask_inv)

# Take only region of logo from logo image.
img2_fg = cv2.bitwise_and(img2,img2,mask = mask)

# Put logo in ROI and modify the main image
dst = cv2.add(img1_bg,img2_fg)
img1[0:rows, 0:cols ] = dst

# 2D卷积
# image_logo = img2.copy()
# kernel = np.ones((5,5),np.float32)/25
# dst = cv2.filter2D(image_logo,-1,kernel)
# blur = cv2.blur(image_logo,(5,5))
# gauss = cv2.GaussianBlur(image_logo,(5,5),0)
# median = cv2.medianBlur(image_logo,5)
# bilateral = cv2.bilateralFilter(image_logo,9,75,75)
# kernel = np.ones((3,3),np.uint8)
# erosion = cv2.erode(image_logo,kernel,iterations = 1)


img_th = cv2.imread('/Users/amen/data/Learning/PythonTest/images/th.jpg')
kernel = np.ones((3,3),np.uint8)
closing = cv2.morphologyEx(img_th, cv2.MORPH_CLOSE, kernel)
# cv2.imshow('res12',closing)

bg_color = [197, 102, 6]
threshold = 3000
def calc_diff(pixel):
    return (pixel[0]-bg_color[0])**2 + (pixel[1]-bg_color[1])**2 + (pixel[2]-bg_color[2])**2

def remove_bg():
    image_path = '/Users/amen/data/Learning/PythonTest/images/th.jpg'
    logo = cv2.imread(image_path)
    logo = cv2.cvtColor(logo, cv2.COLOR_BGR2BGRA) #将图像转成带透明通道的BGRA格式
    h, w = logo.shape[0:2]
    for i in range(h):
        for j in range(w):
            if calc_diff(logo[i][j]) < threshold:
            #若果logo[i][j]为背景，将其颜色设为白色，且完全透明
                logo[i][j][0] = 255
                logo[i][j][1] = 255
                logo[i][j][2] = 255
                logo[i][j][3] = 0
    # cv2.imshow('res13',logo)
    return logo
# remove_bg()

# cv2.waitKey(0)
# cv2.destroyAllWindows()

class Picture(Scatter):

    source = StringProperty(None)
    
    def onpress(self):
        print("key=1, val=2")

class PicturesApp(App):

    def build(self):
        # pil_im = Image.open("/Users/amen/data/Learning/PythonTest/images/th.jpg")
        # print(pil_im.size,pil_im.format,pil_im.mode)
        # pil_im.resize(200,200)
        # pil_im.save("./images/th.jpg") 
        # pil_im = Image.open("./images/th.jpg")
        # print(pil_im.size,pil_im.format,pil_im.mode)

         # the root is created in pictures.kv
        root = self.root

        # get any files into images directory
        curdir = dirname(__file__)
        # for filename in glob(join(curdir, 'images', '*')):
        filename = curdir+'/images/lean.jpg'

        try:
            # load the image
            # picture = Picture(source=filename, rotation=randint(-30, 30))
            picture = Picture(source=filename, rotation=0)
            # add to the main field
            root.add_widget(picture)
        except Exception as e:
            Logger.exception('Pictures: Unable to load <%s>' % filename)  

    def on_pause(self):
        return True

if __name__ == '__main__':
    PicturesApp().run()
