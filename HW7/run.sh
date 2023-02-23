gcc main.c linked_list.c -lpthread
echo "compiled successfully"
./a.out --num_threads=2 --num_iterations=6
mv result.txt result_1.txt
echo "--num_threads=2 --num_iterations=6 successful"
./a.out --num_threads=4 --num_iterations=8
mv result.txt result_2.txt
echo "--num_threads=4 --num_iterations=8 successful"
./a.out --num_threads=8 --num_iterations=10
mv result.txt result_3.txt
echo "--num_threads=8 --num_iterations=10 successful"
./a.out --num_threads=16 --num_iterations=12
mv result.txt result_4.txt
echo "--num_threads=16 --num_iterations=12 successful"
python3 script.py