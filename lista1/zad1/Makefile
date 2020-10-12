default: choinka.out

%: %.cpp
	g++ -I. $< -o $@ common/shader.cpp -lGLEW  -lGL -lglfw 
clean:
	rm a.out *.o *~ choinka.out