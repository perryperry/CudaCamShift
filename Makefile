gpu: gpuMerge.o gpuMain.o main.o RegionOfInterest.o UcharSerialCamShift.o cpuMain.o timing.o
	nvcc -arch=sm_30 gpuMerge.o gpuMain.o timing.o RegionOfInterest.o UcharSerialCamShift.o cpuMain.o main.o -o gpu `pkg-config opencv --cflags --libs`
gpuMain.o: gpuMain.cu
	nvcc -arch=sm_30 -c gpuMain.cu -I/usr/local.cuda-7.0/samples/common -I../../common/inc
gpuMerge.o:	gpuMerge.cu
	nvcc -arch=sm_30 -c gpuMerge.cu -I/usr/local.cuda-7.0/samples/common -I../../common/inc
timing.o: timing.c
	gcc -c timing.c
RegionOfInterest.o: CPU/RegionOfInterest.cpp
	g++ -c CPU/RegionOfInterest.cpp `pkg-config opencv --cflags --libs`
UcharSerialCamShift.o: CPU/UcharSerialCamShift.cpp
	g++ -c CPU/UcharSerialCamShift.cpp `pkg-config opencv --cflags --libs`
cpuMain.o:	CPU/cpuMain.cpp
	g++ -c CPU/cpuMain.cpp `pkg-config opencv --cflags --libs`
main.o:	main.cpp
	g++ -c main.cpp `pkg-config opencv --cflags --libs`
clean:
	rm *.o gpu
run:
	./gpu perry.mov input/windows.txt;

