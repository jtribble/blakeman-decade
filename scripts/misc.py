from PIL import Image
import os
from shutil import copyfile

def move_files():
    for i in range(1, 50):
        image_path = "/Users/kyle/git-projects/blakeman-decade/src/assets/2010/sm/2010-%s.jpg" % i
        out_path = "/Users/kyle/git-projects/blakeman-decade/src/assets/2012/sm/2012-%s.jpg" % i
        im = Image.open(image_path)
        im.save(out_path, "JPEG")

def reverse_order(year):
    for size in ['med', 'sm']:
        for i in range(1, 49):
            src = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s.jpg" % (year, size, year, i)
            dst = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s-tmp.jpg" % (year, size, year, i)
            os.rename(src, dst)
    for size in ['med', 'sm']:
        for i in range(1, 49):
            src = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s-tmp.jpg" % (year, size, year, i)
            dst = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s.jpg" % (year, size, year, 49 - i)
            os.rename(src, dst)

def copy_files(fromYear, toYear):
    for size in ['med', 'sm']:
        for i in range(1, 49):
            image_path = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s.jpg" % (fromYear, size, fromYear, i)
            out_path = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s.jpg" % (toYear, size, toYear, i)
            copyfile(image_path, out_path)

if __name__ == "__main__":
    reverse_order("2011")
