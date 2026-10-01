# Build the CEO email prioritizer. Usage: make && ./ceo_email_queue tests.txt
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
SRCS     := main.cpp CeoInbox.cpp Email.cpp
TARGET   := ceo_email_queue

$(TARGET): $(SRCS) CeoInbox.h Email.h MaxHeap.h
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: clean
