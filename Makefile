TARGET = CustomAllocator
FLAGS  = -g -Wall -O3 -std=c++1z

SRCDIR = Source
INCDIR = Source
OBJDIR = Objects
DBGDIR = Debug
DEPDIR = Dependancies

DBGBIN   = $(DBGDIR)/$(TARGET)
DISASM  := $(DBGBIN:%=$=%.asm)
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPENDS := $(SOURCES:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)

.PHONY: all clean

all: $(DISASM) $(TARGET)

$(TARGET): $(DBGBIN)
	strip $< -o $@

$(DISASM): $(DBGBIN)
	objdump -drCwS -Mintel $< > $@

$(DBGBIN): $(OBJECTS)
	g++ $+ -o $@

-include $(DEPENDS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/%.d
	g++ -c $(FLAGS) -I$(INCDIR) $< -o $@ -MMD -MP -MF $(word 2,$^)

$(DEPENDS): ;

$(DBGBIN): | $(DBGDIR)
$(OBJECTS): | $(OBJDIR) $(DEPDIR)

$(DBGDIR) $(OBJDIR) $(DEPDIR):
	mkdir $@

clean:
	rm -f $(TARGET) $(DBGDIR)/* $(OBJDIR)/* $(DEPDIR)/*
