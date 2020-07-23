all: lib/lorasender_pb2.py lorasender.pb.h

lib/lorasender_pb2.py : lorasender.proto
	protoc --python_out=lib lorasender.proto

lorasender.pb.h : lorasender.proto
	./protoc-nano lorasender.proto