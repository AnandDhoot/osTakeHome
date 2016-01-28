echo "Question 2 :::::::::::"
for x in {1,5,10,12,15,20,25}; do
echo $x >>q2.log
 ./multi-client $1 5000 $x 60 0 random >>q2.log
done;
echo "Question 3 :::::::::::"
for x in {1,5,10,12,15,20,25,30}; do
echo $x >>q3.log
 ./multi-client $1 5000 $x 60 1 random >>q3.log
done;