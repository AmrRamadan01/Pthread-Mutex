for i in {1..10}
do
  echo "Test(" $i ") ................................................. :-" >> final_result.out
  ./caltrain >> final_result.out
done