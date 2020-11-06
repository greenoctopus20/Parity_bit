#give PN the name of your c file wihich contains main
PN= main

CFLAGS=-Wall  -Isrc 

SHARED=src/process.c 
                                 
TEST=$(SHARED) \
             ../Unity/unity.c \

$(PN): 
	gcc $(CFLAGS) src/$(PN).c $(SHARED) -o $(PN) 

#$(PN)Test: 
#	gcc $(CFLAGS) test/$(PN)Test.c $(TEST) -o $(PN)Test 

#test: $(PN)Test
#	./$(PN)Test
    
clean:
	rm -rf $(PN)Test $(PN) 

#valgrind: 
#	valgrind ./$(PN)
