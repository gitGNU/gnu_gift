#!/bin/bash
# first argument: path of images to thumbnail

target=`dirname $1`/`basename $1`
echo $target
cd $target
for each in `ls -1`; do {
convname=`echo $each | sed "s/[.]/_thumbnail_/"`;
if [ ! -f "${target}_thumbnails/${convname}.jpg" ]; then
    {   
        echo converting $each;
        convert -geometry 128x128 ${each} ../${target}_thumbnails/${convname}.jpg;
    }
fi
}
done;
