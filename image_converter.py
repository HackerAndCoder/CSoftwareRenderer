from PIL import Image
import sys, struct

args = sys.argv

if (len(args) < 2):
    print(f"No image args found.")
    exit()

del args[0]
for path in args:
    try:
        img = Image.open(path)
    except:
        print(f"Invalid path: {path}")
        continue
    
    #img.show()
    
    data = img.getdata()
    width = img.width
    height = img.height
    size = width * height
    
    # data is a 1D array of tupes (r, g, b, a)
    data = list(data)
    
    d = []
    
    d.append(width) 
    d.append(height)
    for item in data:
        #output += f"{bytes(item[0])}{bytes(item[1])}{bytes(item[2])}"
        d.append(item[0])
        d.append(item[1])
        d.append(item[2])
    
    with open(path.split(".")[0] + ".bin", "wb") as f:
        f.write(struct.pack(f'{width*height*3+2}i', *d))