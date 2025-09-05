CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc -I/opt/anaconda3/include
LDFLAGS = -L/opt/anaconda3/lib -Wl,-rpath,/opt/anaconda3/lib -lgtest -lgtest_main -pthread

SRCDIR = src
TESTDIR = tests
BUILDDIR = build

SOURCES = $(SRCDIR)/OrderBookEngine.cpp $(SRCDIR)/PriceLevel.cpp
TEST_SOURCES = $(TESTDIR)/test_orderbook_engine.cpp $(TESTDIR)/test_price_level.cpp

all: test

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

test: $(BUILDDIR) $(BUILDDIR)/tests
	$(BUILDDIR)/tests

$(BUILDDIR)/tests: $(SOURCES) $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(TEST_SOURCES) -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILDDIR)

.PHONY: all test clean