XX = g++	#xx指定编译器
CFLAGS = -g	#指定编译的参数,-g表示可debug
CFLAGS += -std=c++11 
CFLAGS += -lpthread
#CLIBS = -mutex		#指定链接时需要的链接库
INCLUDE = $(wildcard ./*.h)	#指定头文件
SOURCES = $(wildcard ./*.cpp)	#指定源文件
SOURCES += $(wildcard ../robot_base/*.cpp)
SOURCES += $(wildcard ../message_data/*.cpp)
SOURCES += $(wildcard ../thread_pool/*.cpp)


TARGET = gobang_test
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

$(TARGET) : $(OBJECTS)
	$(XX) $(CFLAGS) $^ -o $@ $(CLIBS)

$(OBJECTS) : %.o : %.cpp
	$(XX) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm -rf *.o $(TARGET)  

