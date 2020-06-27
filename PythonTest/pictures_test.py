
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

# 查找图片轮廓
def preprocess(save_dir,gray):
    # 1. Sobel算子，x方向求梯度
    sobel = cv2.Sobel(gray, cv2.CV_8U, 1, 0, ksize = 3)
    # 2. 二值化
    ret, binary = cv2.threshold(sobel, 0, 255, cv2.THRESH_OTSU+cv2.THRESH_BINARY)
    
    #gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(gray,190,255,cv2.THRESH_BINARY)
    cv2.imwrite(save_dir+"thresh.jpg", thresh)

    # 3. 膨胀和腐蚀操作的核函数
    element1 = cv2.getStructuringElement(cv2.MORPH_RECT, (30, 9))
    element2 = cv2.getStructuringElement(cv2.MORPH_RECT, (24, 6))
 
    # 4. 膨胀一次，让轮廓突出
    dilation = cv2.dilate(binary, element2, iterations = 1)
 
    # 5. 腐蚀一次，去掉细节，如表格线等。注意这里去掉的是竖直的线
    erosion = cv2.erode(dilation, element1, iterations = 1)
 
    # 6. 再次膨胀，让轮廓明显一些
    dilation2 = cv2.dilate(erosion, element2, iterations = 3)
 
    # 7. 存储中间图片 
    cv2.imwrite(save_dir+"binary.png", binary)
    cv2.imwrite(save_dir+"dilation.png", dilation)
    cv2.imwrite(save_dir+"erosion.png", erosion)
    cv2.imwrite(save_dir+"dilation2.png", dilation2)
 
    return dilation2
 
def findTextRegion(img):
    region = []
 
    # 1. 查找轮廓
    contours, hierarchy = cv2.findContours(img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
 
    # 2. 筛选那些面积小的
    for i in range(len(contours)):
        cnt = contours[i]
        # 计算该轮廓的面积
        area = cv2.contourArea(cnt) 
 
        # 面积小的都筛选掉
        if(area < 50000):
            continue
 
        # 轮廓近似，作用很小
        epsilon = 0.001 * cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, epsilon, True)
 
        # 找到最小的矩形，该矩形可能有方向
        rect = cv2.minAreaRect(cnt)
        print("rect is: ")
        print(rect)

        # box是四个点的坐标
        box = cv2.boxPoints(rect)
        box = np.int0(box)
 
        # 计算高和宽
        height = abs(box[0][1] - box[2][1])
        width = abs(box[0][0] - box[2][0])
 
        # # 筛选那些太细的矩形，留下扁的
        # if(height > width * 1.2):
        #     continue
 
        region.append(box)
        print("region is: ")
        print(region)
    return region
  
def detect(save_dir,img):
    # 1.  转化成灰度图
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
 
    # 2. 形态学变换的预处理，得到可以查找矩形的图片
    dilation = preprocess(save_dir,gray)
 
    # 3. 查找和筛选文字区域
    region = findTextRegion(dilation)
 
    # 4. 用绿线画出这些找到的轮廓
    for box in region:
        cv2.drawContours(img, [box], 0, (255, 0, 0), 2)
 
    # cv2.namedWindow("img", cv2.WINDOW_NORMAL)
    cv2.imshow("img", img)
 
    # 带轮廓的图片
    cv2.imwrite(save_dir+"contours.png", img)
 
    cv2.waitKey(0)
    cv2.destroyAllWindows()

# cv2.waitKey(0)
# cv2.destroyAllWindows()

class Picture(Scatter):

    source = StringProperty(None)
    
    def onpress(self):
        print("key=1, val=2")
                # 读取文件
        imagePath = '/Users/amen/data/Learning/PythonTest/images/lean.jpg'        
        img = cv2.imread(imagePath)
        save_dir = "./PythonTest/images/out/"
        result = img.copy()
    
        gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

        ret,thresh = cv2.threshold(gray,190,255,cv2.THRESH_BINARY)
        cv2.imwrite(save_dir+"thresh.jpg", thresh)

        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
        
        opened = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)
        cv2.imwrite(save_dir+"opened.jpg", opened)
        
        eroded = cv2.erode(opened, kernel)
        cv2.imwrite(save_dir+"eroded.jpg", eroded)
        
        contours, hierarchy = cv2.findContours(eroded,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        
        color = (0, 255, 0)
        for c in contours:
            x, y, w, h = cv2.boundingRect(c)
            cv2.rectangle(img, (x, y), (x + w, y + h), color, 1)
            temp = result[y:(y + h), x:(x + w)]
            # cv2.imwrite("/Users/amen/data/Learning/PythonTest/images/result/" + str(x) + ".jpg", temp)
        
        cv2.imwrite(save_dir+"result.jpg", img)
    
        detect(save_dir,img)

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


if __name__ == '__main__':
    PicturesApp().run()
