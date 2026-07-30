SUBDIRS = $(shell find . -maxdepth 2 -mindepth 2 -name "makefile" -printf "%h\n" | sed 's|^\./||')
GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m

.PHONY: all clean

all:
	@for dir in $(SUBDIRS); do \
		echo "$(GREEN)>>> Building $$dir <<<$(NC)"; \
		if $(MAKE) -C $$dir; then \
			echo "$(GREEN)✓ $$dir built successfully$(NC)"; \
		else \
			echo "$(RED)✗ $$dir build failed$(NC)"; \
			exit 1; \
		fi; \
	done
	@echo "$(GREEN)==================================$(NC)"
	@echo "$(GREEN)All projects built successfully!$(NC)"

clean:
	@for dir in $(SUBDIRS); do \
		echo "$(GREEN)Cleaning $$dir...$(NC)"; \
		$(MAKE) -C $$dir clean; \
	done
