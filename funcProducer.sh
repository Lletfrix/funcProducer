t='.txt'
fc='_full.c'
c='.c'
png='.png'
img='Img'

t=$1$t
c=$1$c
fc=$1$fc
png=$1$png
img=$1$img

mkdir ./Produced/$1
mv ./Pending/$png $png
convert $png $t
mv $png $1/$png
./img2func $t $fc full
./img2func $t $c func
mv $t ./Produced/$1/$t
mv $c ./Produced/$1/$c
mv $fc ./Produced/$1/$fc
