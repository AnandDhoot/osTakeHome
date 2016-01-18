rm q*.log
echo "Question 2 :::::::::::"
for x in {1,5,10,15,20}; do
echo $x >>q2.log
 ./multi-client $1 5000 $x 120 0 random >>q2.log
done;
echo "Question 3 :::::::::::"
for x in {1,5,10,15,20,25}; do
echo $x >>q3.log
 ./multi-client $1 5000 $x 120 1 random >>q3.log
done;
echo "Question 4 :::::::::::"
for x in {1,5,10,15,20}; do
echo $x >>q4.log
./multi-client $1 5000 $x 120 0 fixed >>q4.log
done;