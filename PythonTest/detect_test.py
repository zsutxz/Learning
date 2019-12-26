import cv2
import numpy as np
 
def preprocess(gray):
    # 1. Sobel算子，x方向求梯度
    sobel = cv2.Sobel(gray, cv2.CV_8U, 1, 0, ksize = 3)
    # 2. 二值化
    ret, binary = cv2.threshold(sobel, 0, 255, cv2.THRESH_OTSU+cv2.THRESH_BINARY)
    
    #gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(gray,190,255,cv2.THRESH_BINARY)
    cv2.imwrite("thresh.jpg", thresh)

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
    cv2.imwrite("binary.png", binary)
    cv2.imwrite("dilation.png", dilation)
    cv2.imwrite("erosion.png", erosion)
    cv2.imwrite("dilation2.png", dilation2)
 
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
 
 
def detect(img):
    # 1.  转化成灰度图
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
 
    # 2. 形态学变换的预处理，得到可以查找矩形的图片
    dilation = preprocess(gray)
 
    # 3. 查找和筛选文字区域
    region = findTextRegion(dilation)
 
    # 4. 用绿线画出这些找到的轮廓
    for box in region:
        cv2.drawContours(img, [box], 0, (255, 0, 0), 2)
 
    # cv2.namedWindow("img", cv2.WINDOW_NORMAL)
    cv2.imshow("img", img)
 
    # 带轮廓的图片
    cv2.imwrite("contours.png", img)
 
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':


    img = cv2.imread(imagePath)
    result = img.copy()
 
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
        # 读取文件
    imagePath = '/Users/amen/data/Learning/PythonTest/images/th.jpg'
    ret,thresh = cv2.threshold(gray,190,255,cv2.THRESH_BINARY)
    cv2.imwrite("thresh.jpg", thresh)

    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
    
    opened = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)
    cv2.imwrite("opened.jpg", opened)
    
    eroded = cv2.erode(opened, kernel)
    cv2.imwrite("eroded.jpg", eroded)
    
    contours, hierarchy = cv2.findContours(eroded,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    
    color = (0, 255, 0)
    for c in contours:
        x, y, w, h = cv2.boundingRect(c)
        cv2.rectangle(img, (x, y), (x + w, y + h), color, 1)
        temp = result[y:(y + h), x:(x + w)]
        # cv2.imwrite("/Users/amen/data/Learning/PythonTest/images/result/" + str(x) + ".jpg", temp)
    
    cv2.imwrite("result.jpg", img)
    # detect(img)
