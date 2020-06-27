
import pytesseract
from PIL import Image

pytesseract.pytesseract.tesseract_cmd = r"C:\Program Files (x86)\Tesseract-OCR\tesseract.exe"

image = Image.open("F:/project/Learning/PythonTest/test2.jpg")
text = pytesseract.image_to_string(image,lang='chi_sim')
print(text)
