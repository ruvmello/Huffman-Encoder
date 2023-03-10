# Dit script test het volledige programma op verschillende, soms random, inputs
PAD=$1

echo "Integration test 1: reddit.com"
curl -s "https://www.reddit.com/" > input
cat input | $PAD -c 4,2 | $PAD -d > output
diff -s input output
rm input output

echo "Integration test 2: random picture (https://picsum.photos/)"
wget -q -O input "https://picsum.photos/500"
cat input | $PAD -c 5,2 | $PAD -d > output
diff -s input output
rm input output

echo "Integration test 3: random text"
curl -s "https://randomtextgenerator.com/" > input
cat input | $PAD -c 6,2 | $PAD -d > output
diff -s input output
rm input output

echo "Integration test 4: facebook.com"
curl -s "https://www.facebook.com/" > input
cat input | $PAD -c 7,2 | $PAD -d > output
diff -s input output
rm input output

echo "Integration test 5: youtube.com"
curl -s "https://www.youtube.com/" > input
cat input | $PAD -c 8,2 | $PAD -d > output
diff -s input output
rm input output