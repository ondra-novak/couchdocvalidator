CXXFLAGS =	-O3 -g -Wall -fmessage-length=0 -std=c++11

OBJS =		CouchDocValidator.o

LIBS = -limtjson

TARGET =	bin/couch_doc_validator

$(TARGET):	$(OBJS) | bin
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

bin:
	mkdir bin

clean:
	rm -f $(OBJS) $(TARGET)
