from PIL import Image
import os
from os import listdir
from os.path import isfile, join
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
            src = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s.jpg" % (
                year, size, year, i)
            dst = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s-tmp.jpg" % (
                year, size, year, i)
            os.rename(src, dst)
    for size in ['med', 'sm']:
        for i in range(1, 49):
            src = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s-tmp.jpg" % (
                year, size, year, i)
            dst = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s.jpg" % (
                year, size, year, 49 - i)
            os.rename(src, dst)


def copy_files(fromYear, toYear):
    for size in ['med', 'sm']:
        for i in range(1, 49):
            image_path = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s.jpg" % (
                fromYear, size, fromYear, i)
            out_path = "/Users/kyle/git-projects/blakeman-decade/src/assets/%s/%s/%s-%s.jpg" % (
                toYear, size, toYear, i)
            copyfile(image_path, out_path)


def ensure_dir(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)


def resize_and_move_raw_edits():
    years = ['2008', '2009', '2010', '2011', '2012',
             '2013', '2014', '2015', '2016', '2017']
    raw_path = 'raw_edits'
    ensure_dir('/Users/kyle/git-projects/blakeman-decade/src/assets/')
    for year in years:
        ensure_dir(
            '/Users/kyle/git-projects/blakeman-decade/src/assets/{}/'.format(year))
        ensure_dir(
            '/Users/kyle/git-projects/blakeman-decade/src/assets/{}/{}'.format(year, 'sm'))
        ensure_dir(
            '/Users/kyle/git-projects/blakeman-decade/src/assets/{}/{}'.format(year, 'med'))
        year_path = raw_path + "/" + year
        image_paths = [f for f in listdir(
            year_path) if isfile(join(year_path, f))]
        image_paths = map(lambda x: join(year_path, x), image_paths)
        for im_path in image_paths:
            i = int(im_path.split("-")[-1].split(".")[0])
            im_sm = Image.open(im_path)
            im_sm.thumbnail((500, 500), Image.ANTIALIAS)
            im_med = Image.open(im_path)
            im_med.thumbnail(
                (6000, 6000), Image.ANTIALIAS)
            im_sm.save(
                '/Users/kyle/git-projects/blakeman-decade/src/assets/{}/{}/{}-{}.jpg'.format(year, 'sm', year, i), "JPEG")
            im_med.save(
                '/Users/kyle/git-projects/blakeman-decade/src/assets/{}/{}/{}-{}.jpg'.format(year, 'med', year, i), "JPEG")


if __name__ == "__main__":
    resize_and_move_raw_edits()
