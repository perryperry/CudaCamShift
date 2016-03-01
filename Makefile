gpu: gpuMerge.o  gpuMain.o main.o timing.o
	nvcc -arch=sm_30 gpuMerge.o gpuMain.o timing.o main.o -o gpu `pkg-config opencv --cflags --libs`
gpuMain.o: gpuMain.cu
	nvcc -arch=sm_30 -c gpuMain.cu -I/usr/local.cuda-7.0/samples/common -I../../common/inc
gpuMerge.o:	gpuMerge.cu
	nvcc -arch=sm_30 -c gpuMerge.cu -I/usr/local.cuda-7.0/samples/common -I../../common/inc
timing.o: timing.c
	gcc -c timing.c
main.o:	main.cpp
	g++ -c main.cpp `pkg-config opencv --cflags --libs`
clean:
	rm *.o gpu
run:
	./gpu perry.mov input/windows.txt;

