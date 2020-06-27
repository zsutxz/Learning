import cv2
import numpy as np

# 读取文件
imagePath = '/Users/amen/data/Learning/PythonTest/images/th.jpg'
image = cv2.imread(imagePath)
cv2.imshow("orign", image) #展示图片

retval,tresh=cv2.threshold(image,136,255,cv2.THRESH_BINARY)
cv2.imshow("tresh", tresh) #展示图片

#灰度图片
gray = cv2.cvtColor(tresh, cv2.COLOR_BGR2GRAY)
cv2.imshow("gray", gray) #展示图片

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
cv2.imshow("merge0:",merge)

#两张图片进行减法运算，去掉表格框线
merge2 = cv2.subtract(binary,merge)
cv2.imshow("merge2:",merge2)
   
#识别黑白图中的白色交叉点，将横纵坐标取出
#标识交点
bitwiseAnd = cv2.bitwise_and(dilatedcol,dilatedrow)
# cv2.imshow("dot:",bitwiseAnd)

# cv2.imwrite("my.png",bitwiseAnd) #将二值像素点生成图片保存
ys,xs = np.where(bitwiseAnd>0)
 
mylisty=[] #纵坐标
mylistx=[] #横坐标
 
#通过排序，获取跳变的x和y的值，说明是交点，否则交点会有好多像素值值相近，我只取相近值的最后一点
#这个10的跳变不是固定的，根据不同的图片会有微调，基本上为单元格表格的高度（y坐标跳变）和长度（x坐标跳变）
i = 0
myxs=np.sort(xs)
for i in range(len(myxs)-1):
    if(myxs[i+1]-myxs[i]>10):
        mylistx.append(myxs[i])
    i=i+1
mylistx.append(myxs[i]) #要将最后一个点加入

i = 0
myys=np.sort(ys)
#print(np.sort(ys))
for i in range(len(myys)-1):
    if(myys[i+1]-myys[i]>10):
        mylisty.append(myys[i])
    i=i+1
mylisty.append(myys[i]) #要将最后一个点加入
 
print('mylisty',mylisty)
print('mylistx',mylistx)
 
 
# #循环y坐标，x坐标分割表格
# for i in range(len(mylisty)-1):
#     for j in range(len(mylistx)-1):
#         #在分割时，第一个参数为y坐标，第二个参数为x坐标
#         ROI = image[mylisty[i]+3:mylisty[i+1]-3,mylistx[j]:mylistx[j+1]-3] #减去3的原因是由于我缩小ROI范围
#         cv2.imshow("show:",ROI)

 
#         # special_char_list = '`~!@#$%^&*()-_=+[]{}|\\;:‘’，。《》/？ˇ'
#         # pytesseract.pytesseract.tesseract_cmd = 'E:/Tesseract-OCR/tesseract.exe'
#         # text1 = pytesseract.image_to_string(ROI)  #读取文字，此为默认英文
#         # text2 = ''.join([char for char in text2 if char not in special_char_list])
#         # print('识别分割子图片信息为：'+text1)
#         j=j+1
#     i=i+1

cv2.waitKey(0)