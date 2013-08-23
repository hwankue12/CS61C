# Build file for CS61C Lab 2
# You should not need to edit this file.

# This file requires GNU make and depends on paths on instruction machines.

####

## Variables

# Source files (java code). wildcard selects all files matching a pattern.
SOURCES = $(wildcard *.java)
# Output JAR file
TARGET = mutualfriends.jar
# Extra JARs to have on the classpath when compiling.
CLASSPATH = /home/ff/cs61c/hadoop/hadoop-core.jar:/home/ff/cs61c/hadoop/lib/commons-cli.jar:/home/ff/cs61c/bin/apachecommons/commons-lang3-3.1/commons-lang3-3.1.jar:/home/ff/cs61c/bin/apachecommons/commons-logging-1.1.3/commons-logging-1.1.3.jar
# javac command to use
JAVAC = javac -g -deprecation -Xlint:unchecked -cp $(CLASSPATH)
# jar command to use
JAR = jar

## Make targets

# General form is target: dependencies (targets or files), followed by
# commands to run to build the target from the dependencies.

# Default target.
all: $(TARGET)

$(TARGET): classes $(SOURCES)
	$(JAVAC) -d classes $(SOURCES)
	$(JAR) cf $(TARGET) -C classes .

classes:
	mkdir classes

clean:
	rm -rf classes $(TARGET) *-out
	rm -rf work
	rm -rf intermediate

runh:
	hadoop jar mutualfriends.jar MutualFriends ~cs61c/data/mrlab2/small.seq output-out

.PHONY: clean all
