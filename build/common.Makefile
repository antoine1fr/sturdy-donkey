include build/runtime.Makefile
include build/shaders.Makefile

all: $(DYNAMIC_TARGET) $(STATIC_TARGET) $(SHADERS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(DYNAMIC_TARGET) $(STATIC_TARGET) $(SHADERS)

re: fclean all

.PHONY: all clean fclean re
