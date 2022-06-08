default: build_encoder build_decoder
	
build_encoder:
	g++ encoder.cpp -I/home/sergey/tensorflow-master -I/usr/local/include/opencv4 -L/home/sergey/tensorflow-master/tensorflow/lite/tools/make/gen/linux_x86_64/lib -ltensorflow-lite -lrt -ldl -pthread -lflatbuffers -lopencv_highgui -lopencv_core -lopencv_imgcodecs -o encoder
	
build_decoder:
	g++ decoder.cpp -I/home/sergey/tensorflow-master -I/usr/local/include/opencv4 -L/home/sergey/tensorflow-master/tensorflow/lite/tools/make/gen/linux_x86_64/lib -ltensorflow-lite -lrt -ldl -pthread -lflatbuffers -lopencv_highgui -lopencv_core -lopencv_imgcodecs -o decoder
