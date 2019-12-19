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

#遍历图片，效率低,不推荐使用
def xgtp():
    global imgviewx
    gx,kx,tx=imgviewx.shape#得到像素高度，像素宽度，通道数

    for g in range(0,gx):
        for k in range(0,kx):  #这里得到的是每个像素点，每个点由RGB三色构成
            if(k>0 and k<100):
                imgviewx[g,k,0]=0       # B
                imgviewx[g,k,1]=255       #  G
                imgviewx[g,k,2]=255        #  R
            else:
                imgviewx[g, k, 0] = imgviewx[g, k, 0] #获取到原来的值
                imgviewx[g, k, 1] = imgviewx[g, k, 1]
                imgviewx[g, k, 2] = imgviewx[g, k, 2]
    cv2.imshow("第二个图形窗口",imgviewx)
 
 #创建一个图形，使用np,所以效率高
def cjtx():
    # 初始化像素点值全为0 （rgb都为零，所以是黑色）
    #参数：（[高，宽，通道数],每个通道占的位数（一个字节））
    imgx=np.zeros([400,600,3],np.uint8)

    #初始化像素点值为全为1
    #imgx[110:130,50:70,2]表示一个范围：[高度起始点：高度结束点，宽度起始点：宽度结束点，哪个通道]，起始点均以左上角
    #imgx[:,:,0]=np.ones([400,600],np.uint8)*255  #最终结果为第一个通道（B)全为255，所以是蓝色
    imgx[110:150,50:90,1]=np.ones([40,40],np.uint8)*255
    cv2.imshow("第二个图形窗口",imgx)

#图片区域处理
def pictureArea():
    global imgviewx
    #得到截图
    areax=imgviewx[110:512,200:512]
    #将图片由RGB（3通道）转换为灰度（2通道）
    areax=cv2.cvtColor(areax,cv2.COLOR_BGR2GRAY)
    #将图片有2通道还原成3通道，但色彩不能还原
    areax2=cv2.cvtColor(areax,cv2.COLOR_GRAY2RGB)
    #处理后的区域写到原图上
    imgviewx[110:512,200:512]=areax2
    #显示截图
    cv2.imshow("area",imgviewx)

#泛洪填充，相似像素填充
def fill_color():
    global imgviewx
    h,w,t=imgviewx.shape
    #必要参数
    maskx=np.zeros([h+2,w+2],np.uint8)
    #参数接收：（图片信息，必要参数，参考点位置坐标，填充的颜色，查找范围：最低像素（参考减所写），查找范围：最高像素（参考加所写），全部填充）
    cv2.floodFill(imgviewx,maskx,(150,200),(255,255,0),(100,100,100),(50,50,50),cv2.FLOODFILL_FIXED_RANGE)
    cv2.imshow("area",imgviewx)

#通道分离与合并
def tongdao():
    global imgviewx
    b,g,r=cv2.split(imgviewx)#通道分离
    cv2.imshow("bb",b)#通道图单独显示
    cv2.imshow("gg",g)
    cv2.imshow("rr",r)

    imgviewx[:,:,1]=135 #改变单个通道（0,1,2 => B,G,R)
    cv2.imshow("chang red ",imgviewx)

    imgviewx=cv2.merge([b,g,r])#合并通道

#像素运算
def pixel_operation():
    #读入两张大小和通道相同的图片
    img1=cv2.imread("F://project//Learning//PythonTest//images//th.jpg")
    img2=cv2.imread("F://project//Learning//PythonTest//images//back.jpg")
    print(img1.shape, "=====", img2.shape)
    # 创建一个大小可调整的窗口
    cv2.namedWindow("operation", cv2.WINDOW_NORMAL)
    cv2.imshow("img111", img1)
    cv2.imshow("img222",img2)
    #处理图片
    #像素点相加，如0（黑色），255（白色），0+255=255（白色），超过255还是白色
    # imgoperation=cv2.add(img1,img2)
    #像素相减，如0（黑色），255(白色），0-255=-255=0（黑色）
    # imgoperation=cv2.subtract(img1,img2)
    #像素相乘，255(白色），0/255=0（黑色）
    #imgoperation=cv2.divide(img1,img2)
    #像素相乘，255(白色），0*255=0（黑色）
    #imgoperation=cv2.multiply(img2,img1)
    #像素与,二进制与，如0与255为00000000&11111111=00000000
    imgoperation=cv2.bitwise_and(img1,img2)
    #像素或
    # imgoperation=cv2.bitwise_or(img1,img2)

    #显示处理后的图片
    cv2.imshow("operation", imgoperation)

 #直方图均衡化，对比度改变
def equalization_rgb(imgtu):
    #只能使用灰度图片
    imgx=cv2.cvtColor(imgtu,cv2.COLOR_RGB2GRAY)#转换为灰度
 
    #默认参数，自接使用
    #imgtu=cv2.equalizeHist(imgx)#均衡化
 
     #可修改参数clipLimit的值得到不一样效果
    chanlx=cv2.createCLAHE(clipLimit=30.0,tileGridSize=(8,8))
    imgtu=chanlx.apply(imgx)

    cv2.imshow("equalization",imgtu)#显示
 

#直方图比较，图片相似度比较，遍历像素点，速度慢慢
def create_compara(imgtu):
    h,w,t=imgtu.shape
    rgbx=np.zeros([16*16*16,1],np.float32)
    bsize=256/16
    for row in range(h):
        for col in range(w):
            b=imgtu[row,col,0]
            g=imgtu[row,col,1]
            r=imgtu[row,col,2]
            index=np.int(b/bsize)*16*16+np.int(g/bsize)*16+np.int(r/bsize)
            rgbx[np.int(index),0]=rgbx[np.int(index),0]+1
    return rgbx

def compare_ing():
    img1=cv2.imread("F://project//Learning//PythonTest//images//th.jpg")
    img2=cv2.imread("F://project//Learning//PythonTest//images//back.jpg")
    hist1=create_compara(img1)
    hist2=create_compara(img2)
    cv2.imshow("img1111",img1)
    cv2.imshow("img2222",img2)
    va1=cv2.compareHist(hist1,hist2,cv2.HISTCMP_BHATTACHARYYA)
    va2=cv2.compareHist(hist1,hist2,cv2.HISTCMP_CORREL)
    va3=cv2.compareHist(hist1,hist2,cv2.HISTCMP_CHISQR)
    print("巴氏距离，越小越相似（0,1）：",va1)
    print("相关性，越接近于1，越相似：",va2)
    print("卡方，越小越相似：",va3)

def templatex(img_target,img_root):
    #模板匹配方法
    #toolx=cv2.TM_SQDIFF_NORMED
    toolx=cv2.TM_CCORR_NORMED
    #toolx=cv2.TM_CCOEFF_NORMED

    h,w=img_target.shape[:2]#获取目标图像的高和宽
    #操作匹配
    result=cv2.matchTemplate(img_root,img_target,toolx)
    #得到区域
    min_x,max_x,min_y,max_y=cv2.minMaxLoc(result)
 
    #获取起始点坐标
    if toolx==cv2.TM_SQDIFF_NORMED:
        tl=min_y
    else:
        tl=max_y
    #获取结束点坐标，其中tl[0]表示起始点x轴值，tl[1]表示y
    br=(tl[0]+w,tl[1]+h)
    #创建一个矩形框，参数（要写到的图片，起始点坐标，结束点坐标，颜色值，厚度）
    cv2.rectangle(img_root,tl,br,(0,0,255),5)
    #显示图片
    cv2.imshow("img_rootxx",img_root)

#读取图像，支持 bmp、jpg、png、tiff 等常用格式
image = cv2.imread("F://project//Learning//PythonTest//images//lean.jpg",cv2.IMREAD_COLOR)

print(image.shape)  #输出：(1080, 1920, 3) 高像素，宽像素，通道数
print(image.size)   # 120000  总通道数=高* 宽* 通道数
print(image.dtype)  # uint8  3个通道每个通道占的位数（8位，一个字节）
# print(np.array(image))   #输出每个像素点的参数（ B , G , R )

#获取图片 B G R 各均值， #(204.46305102040816, 208.50832244897958, 217.29540408163263, 0.0) ，红色部分最多
print(cv2.mean(image))
#获取方差,也会打印均值，可用均值方差都为零判断图片无效
print(cv2.meanStdDev(image))

#创建窗口并显示图像
# cv2.namedWindow("Image")
# 翻转
# image = cv2.flip(image,0)

#.........................................................................
#图片处理
#备份图片
imgviewx=image.copy()

#均值模糊,主要用于去除图片噪点
#读取图片并实现图片的模糊效果，参数：（读取图片，（X轴方向模糊，Y轴方向模糊））
# imgviewx=cv2.blur(imgviewx,(3,3))

#中值模糊，主要用于去除椒盐（烧烤配料）噪点
#参数：（图片信息，模糊值）
# imgviewx=cv2.medianBlur(imgviewx,3)

#普通高斯模糊
#参数：（图片信息，参数1，参数2）参数1和参数2只能设置一个
# imgviewx=cv2.GaussianBlur(imgviewx,(0,0),1)

#保留边缘（像素差），高斯模糊
#参数（图片信息，0，要用怎样的方式（越大则越细），空间复杂度（越大越复杂））
# imgviewx=cv2.bilateralFilter(imgviewx,0,50,6)

# #美颜,美白效果valuex值越大越白
# valuex=10;
# imgviewx=cv2.bilateralFilter(cv2.imread("F://project//Learning//PythonTest//images//lean.jpg"),valuex,valuex * 2,valuex / 2)

# #对比度和亮度调整
# duix=0.5 #对比度
# lightx=0 #亮度
# imgviewx=cv2.addWeighted(imgviewx,duix,np.zeros(imgviewx.shape,imgviewx.dtype),1-duix,lightx)

# #显示文字
# # 参数：图像，文字内容， 坐标( x , y ) ，字体，大小，颜色( B , G ,R )，字体厚度
# #颜色值为0-255
# font = cv2.FONT_HERSHEY_SIMPLEX  # 定义字体
# imgviewx = cv2.putText(imgviewx,"DONG XIAO DONG",(10, 50), font, 1.2, (0, 0, 255), 5)
 
# #像素取反
# imgviewx=cv2.bitwise_not(imgviewx)

# cv2.imshow("Image xixixi",imgviewx)

# cv2.imwrite("F://project//Learning//PythonTest//abc.png",image, [int( cv2.IMWRITE_PNG_COMPRESSION), 5])

# xgtp()
# cjtx()
# pictureArea()
# fill_color()
# tongdao()
# pixel_operation()

# equalization_rgb(imgviewx)#直方图均衡化，提高对比度
# compare_ing()#直方图比较，图片相似度

# 模板匹配
# areax = imgviewx[110:512, 200:512]
# templatex(areax,imgviewx)

# 二值化，黑白图
imgviewx2=imgviewx.copy()
#得到灰度图片
imgviewx2=cv2.cvtColor(imgviewx2,cv2.COLOR_BGR2GRAY)
#二值化图像，黑白图像，只有0和1,0为0,1为255
_,imgviewx2=cv2.threshold(imgviewx2,0,255,cv2.THRESH_BINARY|cv2.THRESH_OTSU)
#二值化方法2
imgviewx2=cv2.adaptiveThreshold(imgviewx2,200,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,25,5)

#显示图片，参数：（窗口标识字符串，imread读入的图像）
cv2.imshow("img222222",imgviewx2)

cv2.waitKey(0)

#释放窗口
cv2.destroyAllWindows()

