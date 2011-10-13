#!/bin/sh

# first argument: path of directory containing images to thumbnail

target=`dirname $1`/`basename $1`
thumbnail_dir=`dirname $1`/`basename $1`_thumbnails

echo "converting images in $target, placing them in $thumbnail_dir."

for each in `find $target -maxdepth 1 -type f|sed "s=\(.*\)/=="`; do {
convname=`echo $each | sed "s/\(.*\)[.]/\1_thumbnail_/"`
if [ ! -f "$thumbnail_dir/${convname}.jpg" ]; then
    {   
        echo converting $each
        convert -geometry 128x128 -quality 100 ${target}/${each} $thumbnail_dir/${convname}.jpg
    }
fi
}
done;
