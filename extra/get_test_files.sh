
rm -r test_files
mkdir test_files

cd ./test_files
touch ./kort_bericht.txt

echo "Hi there," >> ./kort_bericht.txt
echo -n "What are you going to do today?" >> ./kort_bericht.txt

wget -q https://raw.githubusercontent.com/harshilkamdar/trump-tweets/master/trump.txt
mv trump.txt trump2.txt
head -1000 trump2.txt > trump.txt
rm trump2.txt

wget -q -O random_image "https://picsum.photos/500"

cd ..
