
DIR="/media/cdrom"
DIR="/dev"

if [ ! -e $DIR ]; then
  echo "create dir $DIR"
  mkdir -vp $DIR
else
  echo "$DIR exist"
fi


