include build/runtime.Makefile
include build/shaders.Makefile

all: $(TARGET) $(SHADERS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(TARGET) $(SHADERS)

re: fclean all

.PHONY: all clean fclean re
